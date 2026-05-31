use rand::Rng;
use syn::{parse_quote, Expr, GenericArgument, Lit, LitFloat, LitInt, PathArguments, PathSegment, Type, TypePath};

trait Mutator {
    fn name(&self) -> &str;
    fn mutate(&self, file: &mut syn::File);
}

struct Replace_Const_Value_288;

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
                        path: syn::Path {
                            leading_colon: None,
                            segments,
                            ..,
                        },
                    }) = &**return_type {
                        for segment in segments {
                            if let PathArguments::AngleBracketed(args) = &segment.arguments {
                                for arg in &mut args.args {
                                    if let GenericArgument::Const(ref mut arg) = arg {
                                        let new_const = match arg {
                                            Expr::Lit(lit) => {
                                                let new_value = match lit.lit {
                                                    Lit::Int(int) => {
                                                        let mut rng = rand::thread_rng();
                                                        let new_int: i32 = rng.gen_range(0..100);
                                                        Lit::Int(LitInt::new(
                                                            &new_int.to_string(),
                                                            proc_macro2::Span::call_site(),
                                                        ))
                                                    }
                                                    Lit::Float(float) => {
                                                        let mut rng = rand::thread_rng();
                                                        let new_float: f64 = rng.gen_range(0.0..100.0);
                                                        Lit::Float(LitFloat::new(
                                                            &new_float.to_string(),
                                                            proc_macro2::Span::call_site(),
                                                        ))
                                                    }
                                                    _ => panic!("Unsupported literal type"),
                                                };
                                                parse_quote!(#new_value)
                                            }
                                            _ => panic!("Unsupported expression type"),
                                        };
                                        *arg = new_const;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}