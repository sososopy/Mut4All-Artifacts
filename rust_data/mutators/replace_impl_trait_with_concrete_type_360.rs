use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, LocalInit, Pat, PatType, Path as SynPath,
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

pub struct Replace_Impl_Trait_With_Concrete_Type_360;

impl Mutator for Replace_Impl_Trait_With_Concrete_Type_360 {
    fn name(&self) -> &str {
        "Replace_Impl_Trait_With_Concrete_Type_360"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut type_aliases = Vec::new();

        for item in &mut file.items {
            if let Item::Type(type_item) = item {
                if let Type::ImplTrait(_) = *type_item.ty {
                    type_aliases.push(type_item.ident.clone());
                }
            }
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(LocalInit { expr, .. }) = &local.init {
                            if let Expr::Path(expr_path) = &**expr {
                                if let Some(segment) = expr_path.path.segments.first() {
                                    if type_aliases.contains(&segment.ident) {
                                        let new_type: Type = parse_quote!(Option<(u32, u32)>);
                                        local.pat = Pat::Type(PatType {
                                            attrs: Vec::new(),
                                            pat: Box::new(Pat::Wild(PatWild {
                                                attrs: Vec::new(),
                                                underscore_token: token::Underscore::default(),
                                            })),
                                            colon_token: token::Colon::default(),
                                            ty: Box::new(new_type),
                                        });
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
        "This mutation operator identifies type alias declarations using `impl Trait` and replaces their usage in variable declarations with a concrete type. This ensures that any match expressions using the previous variable correctly pattern against the new concrete type, revealing discrepancies in pattern matching and type handling."
    }
}