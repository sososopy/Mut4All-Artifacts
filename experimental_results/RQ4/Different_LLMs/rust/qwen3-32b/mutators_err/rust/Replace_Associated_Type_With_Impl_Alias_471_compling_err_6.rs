use syn::parse_quote;
use crate::mutator::Mutator;
use syn::Type;
use syn::TypePath;

pub struct Replace_Associated_Type_With_Impl_aalias_471;

impl Mutator for Replace_Associated_Type_With_Impl_aalias_471 {
    fn name(&self) -> &str {
        "Replace_Associated_Type_With_Impl_aalias_471"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                let has_associated_type = item_impl.items.iter().any(|i| {
                    matches!(i, syn::ImplItem::Type(_))
                });

                if has_associated_type {
                    let dummy_t_type: Type = parse_quote! {
                        impl SomeTrait
                    };

                    let dummy_t_item = parse_quote! {
                        type DummyT<t> = #dummy_t_type;
                    };

                    file.items.insert(0, dummy_t_item);

                    let param_name = item_impl.generics.params.iter()
                        .find_map(|param| {
                            if let syn::GenericParam::Type(type_param) = param {
                                Some(type_param.ident.clone())
                            } else {
                                None
                            }
                        });

                    if let Some(param) = param_name {
                        let dummy_t_type_path: TypePath = parse_quote! {
                            DummyT<#param>
                        };

                        let trait_path: TypePath = parse_quote! {
                            SomeTrait<#dummy_t_type_path>
                        };

                        let predicate = parse_quote! {
                            #param: #trait_path
                        };

                        if let Some(where_clause) = &mut item_impl.generics.where_clause {
                            where_clause.predicates.push(predicate);
                        } else {
                            let where_clause = parse_quote! {
                                where #predicate
                            };
                            item_impl.generics.where_clause = Some(where_clause);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}