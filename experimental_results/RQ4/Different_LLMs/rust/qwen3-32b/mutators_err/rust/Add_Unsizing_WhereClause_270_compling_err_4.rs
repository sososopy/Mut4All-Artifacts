use syn::parse_quote;
use crate::mutator::Mutator;
use syn::Ident;

pub struct Add_Unsizing_WhereClause_270;

impl Mutator for Add_Unsizing_WhereClause_270 {
    fn name(&self) -> &str {
        "Add_Unsizing_WhereClause_270"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut existing_traits: Vec<Ident> = file.items.iter()
            .filter_map(|item| {
                if let syn::Item::Trait(trait_item) = item {
                    Some(trait_item.ident.clone())
                } else {
                    None
                }
            })
            .collect();

        let trait_name = if !existing_traits.is_empty() {
            existing_traits.first().unwrap().clone()
        } else {
            let new_trait = parse_quote! {
                trait OtherTrait {}
            };
            file.items.insert(0, syn::Item::Trait(new_trait));
            Ident::new("OtherTrait", proc_macro2::Span::call_site())
        };

        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                let generics = &mut impl_item.generics;
                if !generics.params.is_empty() {
                    if let Some(first_param) = generics.params.first() {
                        if let syn::GenericParam::Type(type_param) = first_param {
                            let param_name = &type_param.ident;

                            let dyn_trait = syn::Type::TraitObject(syn::TypeTraitObject {
                                dyn_token: Some(syn::token::Dyn::default()),
                                bounds: {
                                    let mut bounds = syn::punctuated::Punctuated::new();
                                    bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                                        path: syn::Path::from(trait_name.clone()),
                                        paren_token: None,
                                        modifier: syn::TraitBoundModifier::None,
                                        lifetimes: None,
                                    }));
                                    bounds
                                },
                            });

                            let unsize_bound = {
                                let mut path = syn::Path::from(syn::Ident::new("Unsize", proc_macro2::Span::call_site()));
                                let mut segments = path.segments.into_iter();
                                if let Some(mut segment) = segments.next() {
                                    segment.arguments = syn::PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                                        colon2_token: None,
                                        lt_token: syn::token::Lt::default(),
                                        args: {
                                            let mut args = syn::punctuated::Punctuated::new();
                                            args.push(syn::GenericArgument::Type(dyn_trait));
                                            args
                                        },
                                        gt_token: syn::token::Gt::default(),
                                    });
                                    path.segments = syn::punctuated::Punctuated::from_iter(vec![segment]);
                                }
                                syn::TypeParamBound::Trait(syn::TraitBound {
                                    path,
                                    paren_token: None,
                                    modifier: syn::TraitBoundModifier::None,
                                    lifetimes: None,
                                })
                            };

                            let predicate = syn::WherePredicate::Type(syn::PredicateType {
                                lifetimes: None,
                                bounded_ty: syn::Type::Path(syn::TypePath {
                                    qself: None,
                                    path: syn::Path::from(param_name.clone()),
                                }),
                                colon_token: syn::token::Colon::default(),
                                bounds: syn::punctuated::Punctuated::from_iter(vec![unsize_bound]),
                            });

                            if let Some(where_clause) = &mut generics.where_clause {
                                where_clause.predicates.push(predicate);
                            } else {
                                let mut where_clause = syn::WhereClause {
                                    where_token: syn::token::Where::default(),
                                    predicates: syn::punctuated::Punctuated::from_iter(vec![predicate]),
                                };
                                generics.where_clause = Some(where_clause);
                            }
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