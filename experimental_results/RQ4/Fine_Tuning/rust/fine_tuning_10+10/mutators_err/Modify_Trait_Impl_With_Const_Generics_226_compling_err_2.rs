use crate::mutator::Mutator;

pub struct Modify_Trait_Impl_With_Const_Generics_226;

impl Mutator for Modify_Trait_Impl_With_Const_Generics_226 {
    fn name(&self) -> &str {
        "Modify_Trait_Impl_With_Const_Generics_226"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut function_name = None;
        for item in &file.items {
            if let Item::Fn(ItemFn { sig, .. }) = item {
                if sig.inputs.is_empty() {
                    if let ReturnType::Type(_, box_type) = &sig.output {
                        if let Type::Path(TypePath { path, .. }) = &**box_type {
                            if path.segments.last().unwrap().ident == "usize" {
                                function_name = Some(sig.ident.clone());
                                break;
                            }
                        }
                    }
                }
            }
        }

        if function_name.is_none() {
            function_name = Some(Ident::new("generated_function", Span::call_site()));
            let new_function: ItemFn = parse_quote! {
                fn generated_function() -> usize {
                    42
                }
            };
            file.items.insert(0, Item::Fn(new_function));
        }

        if let Some(fn_name) = function_name {
            for item in &mut file.items {
                if let Item::Impl(item_impl) = item {
                    if let Some((_, path, _)) = &item_impl.trait_ {
                        for segment in &mut path.segments {
                            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                for arg in &mut args.args {
                                    if let GenericArgument::Type(Type::Array(type_array)) = arg {
                                        if let Expr::Path(ExprPath { path: array_path, .. }) = &type_array.len {
                                            if array_path.segments.len() == 1 {
                                                let new_expr: Expr = parse_quote! { #fn_name() };
                                                type_array.len = new_expr;
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
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator modifies trait implementations that use const generics by replacing the const parameter with a function call. It identifies existing functions or introduces a new function to replace the const parameter, ensuring the function has no parameters and returns an appropriate type. This tests the compiler's handling of const generic arguments."
    }
}