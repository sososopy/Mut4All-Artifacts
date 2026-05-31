use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCast, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Replace_Dyn_Trait_Cast_With_Numeric_331;

impl Mutator for Replace_Dyn_Trait_Cast_With_Numeric_331 {
    fn name(&self) -> &str {
        "Replace_Dyn_Trait_Cast_With_Numeric_331"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ReplaceDynCastVisitor;
        impl<'ast> VisitMut for ReplaceDynCastVisitor {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Cast(expr_cast) = expr {
                    if let Type::TraitObject(_) = &*expr_cast.ty {
                        let types = ["u8", "u16", "u32", "u64", "i8", "i16", "i32", "i64", "usize", "isize"];
                        let mut rng = thread_rng();
                        let from_type = types.choose(&mut rng).unwrap();
                        let to_type = types.choose(&mut rng).unwrap();
                        let from_ident = Ident::new(from_type, Span::call_site());
                        let to_ident = Ident::new(to_type, Span::call_site());
                        let tokens = quote! { 0 as #from_ident as #to_ident };
                        *expr = syn::parse2(tokens).unwrap();
                        return;
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }
        let mut visitor = ReplaceDynCastVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}