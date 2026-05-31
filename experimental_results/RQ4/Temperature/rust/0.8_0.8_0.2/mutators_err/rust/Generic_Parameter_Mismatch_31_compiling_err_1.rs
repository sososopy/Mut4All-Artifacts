use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Generic_Parameter_Mismatch_31;

impl Mutator for Generic_Parameter_Mismatch_31 {
    fn name(&self) -> &str {
        "Generic_Parameter_Mismatch_31"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if !func.sig.generics.params.is_empty() {
                    if let ReturnType::Type(_, ref mut ty) = func.sig.output {
                        // Choose a random generic parameter, and another one for the mismatch
                        let mut rng = thread_rng();
                        let params = &func.sig.generics.params;
                        let param_count = params.len();

                        if param_count < 2 {
                            continue; // Need at least two params to create a mismatch
                        }

                        let original_idx = rng.gen_range(0..param_count);
                        let mismatch_idx = (original_idx + 1) % param_count;

                        if let Some(GenericParam::Type(ref original_param)) = params.iter().nth(original_idx) {
                            if let Some(GenericParam::Type(ref mismatch_param)) = params.iter().nth(mismatch_idx) {
                                // Create a new TypePath with the mismatched parameter
                                let new_type_path = TypePath {
                                    qself: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments: {
                                            let mut segments = Punctuated::new();
                                            segments.push(PathSegment {
                                                ident: mismatch_param.ident.clone(),
                                                arguments: PathArguments::None,
                                            });
                                            segments
                                        },
                                    },
                                };
                                *ty = Box::new(Type::Path(new_type_path));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation introduces a mismatch in the use of generic parameters within function signatures. By altering a return type or a parameter from using one generic parameter to an unrelated one, it aims to test the compiler's handling of type mismatches, potentially revealing issues with type inference and generic constraints."
    }
}