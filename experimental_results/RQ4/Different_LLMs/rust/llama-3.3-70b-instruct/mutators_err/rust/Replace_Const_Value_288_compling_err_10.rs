use rand::Rng;
use syn::{parse_quote, Expr, GenericArgument, Lit, LitFloat, LitInt, PathArguments, PathSegment, Type, TypePath, punctuated::Punctuated};

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
                        },
                    }) = &**return_type {
                        for segment in segments {
                            if let PathArguments::AngleBracketed(args) = &segment.arguments {
                                let mut new_args = Punctuated::new();
                                for arg in &args.args {
                                    if let GenericArgument::Const(arg) = arg {
                                        let new_value = match arg {
                                            Expr::Lit(lit) => {
                                                match &lit.lit {
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
                                                }
                                            }
                                            _ => panic!("Unsupported expression type"),
                                        };
                                        new_args.push(GenericArgument::Const(parse_quote!(#new_value)));
                                    } else {
                                        new_args.push(arg.clone());
                                    }
                                }
                                segment.arguments = PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                                    colon2_token: args.colon2_token,
                                    lt_token: args.lt_token,
                                    args: new_args,
                                    gt_token: args.gt_token,
                                });
                            }
                        }
                    }
                }
            }
        }
    }
}