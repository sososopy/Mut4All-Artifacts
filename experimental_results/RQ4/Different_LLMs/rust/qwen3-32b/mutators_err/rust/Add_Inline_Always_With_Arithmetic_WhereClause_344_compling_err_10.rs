use crate::mutator::Mutator;
use syn::parse_quote;
use std::collections::HashSet;

pub struct Add_Inline_Always_With_Arithmetic_WhereClause_344;

impl Mutator for Add_Inline_Always_With_Arithmetic_WhereClause_344 {
    fn name(&self) -> &str {
        "Add_Inline_Always_With_Arithmetic_WhereClause_344"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let has_const_param = func.sig.generics.params.iter().any(|param| {
                    if let syn::GenericParam::Const(_) = param {
                        true
                    } else {
                        false
                    }
                });
                if has_const_param {
                    let inline_attr = parse_quote! {
                        #[inline(always)]
                    };
                    func.attrs.push(inline_attr);
                    
                    if let syn::ReturnType::Type(_, ty) = &func.sig.output {
                        if let syn::Type::Array(array) = &**ty {
                            if let syn::Expr::Path(expr_path) = &array.len {
                                if let Some(ident) = expr_path.path.get_ident() {
                                    let const_param_names: HashSet<String> = func.sig.generics.params.iter()
                                        .filter_map(|param| {
                                            if let syn::GenericParam::Const(c) = param {
                                                Some(c.ident.to_string())
                                            } else {
                                                None
                                            }
                                        })
                                        .collect();
                                    if const_param_names.contains(&ident.to_string()) {
                                        let struct_name = format!("{}_Wrapper", func.sig.ident);
                                        let struct_name = syn::Ident::new(&struct_name, func.sig.ident.span());
                                        
                                        if let Some(first_const) = func.sig.generics.params.iter()
                                            .find(|param| if let syn::GenericParam::Const(_) = param { true } else { false })
                                            .and_then(|param| if let syn::GenericParam::Const(c) = param { Some(c) } else { None }) {
                                            let const_name = &first_const.ident;
                                            let const_type = &first_const.ty;
                                            let array_element_type = &array.elem;
                                            
                                            let new_struct = parse_quote! {
                                                struct #struct_name<const #const_name: #const_type> where [(); { 2 * #const_name }]: {
                                                    data: [#array_element_type; #const_name],
                                                }
                                            };
                                            new_items.push(syn::Item::Struct(new_struct));
                                            
                                            let impl_block = parse_quote! {
                                                impl<const #const_name: #const_type> #struct_name<#const_name> where [(); { 2 * #const_name }]: {
                                                    fn new() -> Self {
                                                        Self {
                                                            data: #func.sig.ident::<{ #const_name }>(),
                                                        }
                                                    }
                                                }
                                            };
                                            new_items.push(syn::Item::Impl(impl_block));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        file.items.extend(new_items);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}