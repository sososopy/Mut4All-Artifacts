use proc_macro2::{Span, Ident};
use quote::*;
use rand::{Rng, thread_rng};
use syn::{
    GenericParam, Lifetime, LifetimeParam, WhereClause, PredicateLifetime,
    punctuated::Punctuated, Item, File, parse_quote,
};

use crate::mutator::Mutator;

pub struct Randomize_Lifetime_Bounds_459;

impl Mutator for Randomize_Lifetime_Bounds_459 {
    fn name(&self) -> &str {
        "Randomize_Lifetime_Bounds_459"
    }
    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut rng = thread_rng();
                let lifetime_ident = Ident::new(&format!("lifetime{}", rng.r#gen::<u32>()), Span::call_site());
                let lifetime_param = LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: Lifetime::new(&format!("'{}", lifetime_ident), Span::call_site()),
                    colon_token: None,
                    bounds: Punctuated::new(),
                };
                func.sig.generics.params.push(GenericParam::Lifetime(lifetime_param));
                func.sig.generics.where_clause = Some(WhereClause {
                    where_token: Default::default(),
                    predicates: {
                        let mut predicates = Punctuated::new();
                        predicates.push(syn::WherePredicate::Lifetime(PredicateLifetime {
                            lifetime: Lifetime::new(&format!("'{}", lifetime_ident), Span::call_site()),
                            colon_token: Default::default(),
                            bounds: {
                                let mut bounds = Punctuated::new();
                                bounds.push(Lifetime::new("'static", Span::call_site()));
                                bounds
                            },
                        }));
                        predicates
                    },
                });
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces random lifetime parameters and bounds into function signatures. By adding unpredictable lifetimes with static bounds, it aims to stress the compiler's lifetime resolution and inference mechanisms, potentially triggering ICEs or assertion failures in complex lifetime scenarios."
    }
}