use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Replace_Dyn_With_Ptr_333;

impl Mutator for Replace_Dyn_With_Ptr_333 {
    fn name(&self) -> &str {
        "Replace_Dyn_With_Ptr_333"
    }
    fn mutate(&self, file: &mut syn::File) {
        // Replace 'dyn T' with '*dyn T' in type annotations
        struct DynToPtrVisitor;
        impl<'ast> VisitMut for DynToPtrVisitor {
            fn visit_type_mut(&mut self, i: &mut Type) {
                if let Type::TraitObject(trait_object) = i {
                    // Replace with a raw pointer to the trait object
                    let new_type = Type::Ptr(syn::TypePtr {
                        star_token: Default::default(),
                        const_token: Default::default(),
                        mutability: Default::default(),
                        elem: Box::new(Type::TraitObject(trait_object.clone())),
                    });
                    *i = new_type;
                }
                visit_mut::visit_type_mut(self, i);
            }
        }
        DynToPtrVisitor.visit_file_mut(file);

        // Insert casts to Box<dyn T> in Box::new calls
        struct BoxNewCastVisitor;
        impl<'ast> VisitMut for BoxNewCastVisitor {
            fn visit_expr_call_mut(&mut self, i: &mut ExprCall) {
                if let Expr::Path(path) = &*i.func {
                    if path.path.is_ident("Box") {
                        if let Some(segment) = path.path.segments.last() {
                            if segment.ident == "new" {
                                // This is a Box::new call
                                // Wrap it with a cast to Box<dyn Send>
                                let cast_expr = parse_quote! {
                                    #i as Box<dyn Send>
                                };
                                *i = cast_expr;
                                return;
                            }
                        }
                    }
                }
                visit_mut::visit_expr_call_mut(self, i);
            }
        }
        BoxNewCastVisitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces all 'dyn T' type annotations with raw pointers '*dyn T' and inserts casts to 'Box<dyn T>' in Box::new calls. This targets trait object unification and codegen phases, exposing ICEs in predicate indexing when dynamic bounds mismatch."
    }
}