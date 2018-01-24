%% compile C code:
mex moving_average.c

%% signal parameters: 
nr_of_samples = 1000;
signal_amplitude = 100;
noise_amplitude = 50;
periods_in_frame = 5;
time_axis = 1:nr_of_samples;
window_size = 50;

%% generate signal
data = (sin(periods_in_frame*time_axis*2*pi/nr_of_samples)+1) ...
    * signal_amplitude;
noise = noise_amplitude*rand(1,nr_of_samples);
signal = data + noise; 

%% cast to uint32
signal = uint32(signal);
window_size = uint32(window_size);

%% call C function
output = moving_average(signal, window_size);

%% display
figure(1);plot(time_axis, signal, time_axis, output );
