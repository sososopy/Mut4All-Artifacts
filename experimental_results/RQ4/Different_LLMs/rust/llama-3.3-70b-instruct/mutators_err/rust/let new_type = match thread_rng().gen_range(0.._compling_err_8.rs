impl Mutator for Generic_Constant_Expression_Swap_352 {
    fn name(&self) -> &str {
        "Generic_Constant_Expression_Swap_352"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for arg in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = arg {
                        if let Type::Path(TypePath {
                            qself: None,
                            path: SynPath {
                                leading_colon: None,
                                segments: segments,
                            },
                        }) = &*pat_type.ty {
                            if segments.len() == 1 && segments[0].ident == "T" {
                                let new_type = match thread_rng().gen_range(0..2) {
                                    0 => parse_quote!(i32),
                                    1 => parse_quote!(String),
                                    _ => unreachable!(),
                                };
                                *pat_type.ty = new_type;
                            }
                        }
                    }
                }
            }
        }
    }
}