use syn::parse_quote;
use crate::mutator::Mutator;
use syn::Ident;
use syn::Item;
use syn::WherePredicate;
use syn::Type;
use syn::TypeParamBound;
use syn::TraitBoundModifier;
use syn::token::Question;
use proc_macro2::Span;
use rand::thread_rng;
use syn::punctuated::Punctuated;
use syn::TraitBound;
use rand::prelude::SliceRandom;

pub struct Introduce_Negated_Const_Trait_Bounds_312;

impl Mutator for Introduce_Negated_Const_Trait_Bounds_312 {
    fn name(&self) -> &str {
        "Introduce_Negated_Const_Trait_Bounds_312"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut all_traits: Vec<Ident> = Vec::new();
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                all_traits.push(trait_item.ident.clone());
            }
        }

        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                if trait_item.attrs.iter().any(|attr| attr.path().is_ident("const_trait")) {
                    for trait_item in &mut trait_item.items {
                        if let syn::TraitItem::Fn(method) = trait_item {
                            let method_sig = &mut method.sig;
                            let has_where_clause = method_sig.generics.where_clause.is_some();
                            let mut found_predicate = false;

                            if let Some(where_clause) = &mut method_sig.generics.where_clause {
                                for predicate in &mut where_clause.predicates {
                                    if let WherePredicate::Type(type_pred) = predicate {
                                        if let Type::Path(type_path) = &type_pred.bounded_ty {
                                            if type_path.path.is_ident("Self") {
                                                if let Some(first_bound) = type_pred.bounds.iter_mut().find_map(|bound| {
                                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                                        Some(trait_bound)
                                                    } else {
                                                        None
                                                    }
                                                }) {
                                                    first_bound.modifier = TraitBoundModifier::Maybe(Question {
                                                        spans: [Span::call_site()],
                                                    });
                                                    found_predicate = true;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            if !found_predicate && !has_where_clause && !all_traits.is_empty() {
                                let mut rng = thread_rng();
                                let selected_trait = all_traits.choose(&mut rng).unwrap().clone();
                                let new_where_clause = syn::WhereClause {
                                    where_token: Default::default(),
                                    predicates: {
                                        let mut preds = Punctuated::new();
                                        let path = parse_quote!(#selected_trait);
                                        let trait_bound = TraitBound {
                                            paren_token: None,
                                            modifier: TraitBoundModifier::Maybe(Question {
                                                spans: [Span::call_site()],
                                            }),
                                            lifetimes: None,
                                            path,
                                        };
                                        let bounds = Punctuated::from_iter(vec![TypeParamBound::Trait(trait_bound)]);
                                        preds.push(WherePredicate::Type(syn::PredicateType {
                                            lifetimes: None,
                                            bounded_ty: parse_quote!(Self),
                                            colon_token: Default::default(),
                                            bounds,
                                        }));
                                        preds
                                    },
                                };
                                method_sig.generics.where_clause = Some(new_where_clause);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces negated const trait bounds in where clauses of const trait methods. It replaces existing trait bounds with `~const` modifiers or adds new where clauses with negated constraints, leveraging existing traits in the code. This tests the compiler's handling of negated const trait constraints in const trait impls, potentially exposing bugs in trait resolution and const evaluation."
    }
}