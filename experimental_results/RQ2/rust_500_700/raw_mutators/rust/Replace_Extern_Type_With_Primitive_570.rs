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

pub struct Replace_Extern_Type_With_Primitive_570;

impl Mutator for Replace_Extern_Type_With_Primitive_570 {
    fn name(&self) -> &str {
        "Replace_Extern_Type_With_Primitive_570"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Item(Item::ForeignMod(foreign_mod)) = stmt {
                        for foreign_item in &mut foreign_mod.items {
                            if let ForeignItem::Static(foreign_static) = foreign_item {
                                if let Type::Ptr(ptr_type) = &mut *foreign_static.ty {
                                    if let Type::Path(type_path) = &*ptr_type.elem {
                                        if type_path.path.segments.len() == 1 {
                                            let segment = &type_path.path.segments[0];
                                            if segment.ident == "T" {
                                                ptr_type.elem = Box::new(Type::Path(TypePath {
                                                    qself: None,
                                                    path: parse_quote!(u32),
                                                }));
                                            }
                                        }
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
        ""
    }
}