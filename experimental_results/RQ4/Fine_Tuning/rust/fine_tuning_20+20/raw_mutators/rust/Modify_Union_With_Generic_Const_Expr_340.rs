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

pub struct Modify_Union_With_Generic_Const_Expr_340;

impl Mutator for Modify_Union_With_Generic_Const_Expr_340 {
    fn name(&self) -> &str {
        "Modify_Union_With_Generic_Const_Expr_340"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Union(item_union) = item {
                let mut has_const_generic = false;
                for param in &item_union.generics.params {
                    if matches!(param, GenericParam::Const(_)) {
                        has_const_generic = true;
                        break;
                    }
                }
                if !has_const_generic {
                    item_union.generics.params.push(parse_quote!(const N: usize));
                }
                for field in item_union.fields.named.iter_mut() {
                    if let Type::Array(type_array) = &mut field.ty {
                        if let syn::Expr::Block(expr_block) = &*type_array.len {
                            if expr_block.block.stmts.len() == 1 {
                                if let syn::Stmt::Expr(expr) = &expr_block.block.stmts[0] {
                                    if let syn::Expr::Call(expr_call) = expr {
                                        if let syn::Expr::Path(expr_path) = &*expr_call.func {
                                            if expr_path.path.is_ident("mem") {
                                                field.ty = parse_quote!([u8; N + const { mem::size_of::<T>() }]);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main" {
                    continue;
                }
                let mut has_union = false;
                for stmt in &item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Pat::Type(pat_type) = &local.pat {
                            if let Type::Path(type_path) = &*pat_type.ty {
                                if type_path.path.segments.len() == 1
                                    && type_path.path.segments[0].ident == "ExampleUnion"
                                {
                                    has_union = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                if has_union {
                    let test_stmt: Stmt = parse_quote! {
                        let _x: ExampleUnion<i32, 4>;
                    };
                    item_fn.block.stmts.push(test_stmt);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets union definitions that use generic const expressions. It adds an additional const generic parameter to the union and modifies the field's constant expression to incorporate this new parameter. The mutator also ensures that any test functions instantiate the union with the new const parameter, increasing the complexity of the const evaluation and testing the compiler's handling of generic const expressions in union contexts."
    }
}