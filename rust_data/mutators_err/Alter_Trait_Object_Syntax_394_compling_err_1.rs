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

pub struct Alter_Trait_Object_Syntax_394;

impl Mutator for Alter_Trait_Object_Syntax_394 {
    fn name(&self) -> &str {
        "Alter_Trait_Object_Syntax_394"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct TraitObjectMutator;

        impl VisitMut for TraitObjectMutator {
            fn visit_local_mut(&mut self, local: &mut Local) {
                if let Some((_, ref mut ty)) = local.ty {
                    if let Type::TraitObject(ref mut trait_object) = **ty {
                        for bound in &mut trait_object.bounds {
                            if let TypeParamBound::Trait(TraitBound { ref mut dyn_token, .. }) = bound {
                                if dyn_token.is_some() {
                                    dyn_token.take();
                                } else {
                                    *dyn_token = Some(token::Dyn { span: Span::call_site() });
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_local_mut(self, local);
            }

            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Cast(ref mut expr_cast) = expr {
                    if let Type::TraitObject(ref mut trait_object) = *expr_cast.ty {
                        for bound in &mut trait_object.bounds {
                            if let TypeParamBound::Trait(TraitBound { ref mut dyn_token, .. }) = bound {
                                if dyn_token.is_some() {
                                    dyn_token.take();
                                } else {
                                    *dyn_token = Some(token::Dyn { span: Span::call_site() });
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }

        let mut visitor = TraitObjectMutator;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets trait objects in variable declarations within function scopes. It alters the syntax of trait objects by toggling between using `dyn* Trait` and `dyn Trait`. The mutation ensures that the type conversion or casting associated with the trait object is adjusted accordingly. This enhances the flexibility of trait object syntax usage in Rust code."
    }
}