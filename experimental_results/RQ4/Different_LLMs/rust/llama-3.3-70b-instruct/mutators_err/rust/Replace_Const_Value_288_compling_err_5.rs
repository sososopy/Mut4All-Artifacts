```rust
impl Mutator for Replace_Const_Value_288 {
    fn name(&self) -> &str {
        "Replace_Const_Value_288"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, return_type) = &func.sig.output {
                    if let Type::Path(TypePath {
                        qself: None,
                        path: SynPath {
                            leading_colon: None,
                            segments,
                            ..
                        },
                        ..
                    }) = &**return_type
                    {
                        for segment in segments {
                            if let PathArguments::AngleBracketed(args) = &segment.arguments {
                                for arg in &mut args.args {
                                    if let GenericArgument::Const(ref mut arg) = arg {
                                        let new_const = if let Expr::Lit(lit) = arg {
                                            let new_value = match lit.lit {
                                                Lit::Int(int) => {
                                                    let mut rng = thread_rng();
                                                    let new_int: i32 =
                                                        rng.gen_range(0..100);
                                                    Lit::Int(syn::LitInt::new(
                                                        &new_int.to_string(),
                                                        Span::call_site(),
                                                    ))
                                                }
                                                Lit::Float(float) => {
                                                    let mut rng = thread_rng();
                                                    let new_float: f64 =
                                                        rng.gen_range(0.0..