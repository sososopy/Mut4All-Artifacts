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

pub struct Replace_Slice_Length_In_Constant_57;

impl Mutator for Replace_Slice_Length_In_Constant_57 {
    fn name(&self) -> &str {
        "Replace_Slice_Length_In_Constant_57"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Const(item_const) = item {
                if let Type::Reference(type_ref) = &*item_const.ty {
                    if let Type::Slice(type_slice) = &*type_ref.elem {
                        let elem_type = &type_slice.elem;
                        if let Expr::Call(expr_call) = &*item_const.expr {
                            for arg in &mut expr_call.args {
                                if let Expr::Array(expr_array) = arg {
                                    let new_len = expr_array.elems.len() + rng.gen_range(1..4);
                                    let mut new_elems = expr_array.elems.clone();
                                    for _ in expr_array.elems.len()..new_len {
                                        let default_expr = match &**elem_type {
                                            Type::Path(type_path) => {
                                                let ident = &type_path.path.segments.last().unwrap().ident;
                                                if ident == "bool" {
                                                    parse_quote!(false)
                                                } else if ident == "i32" || ident == "u32" || ident == "i64" || ident == "u64" || ident == "i8" || ident == "u8" || ident == "i16" || ident == "u16" || ident == "isize" || ident == "usize" {
                                                    parse_quote!(0)
                                                } else if ident == "f32" || ident == "f64" {
                                                    parse_quote!(0.0)
                                                } else if ident == "char" {
                                                    parse_quote!('\0')
                                                } else {
                                                    continue;
                                                }
                                            }
                                            _ => continue,
                                        };
                                        new_elems.push(default_expr);
                                    }
                                    expr_array.elems = new_elems;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets constants with slice types within compound types (like references to structs containing slices). It replaces a constant slice reference with a constant array reference of the same element type but different length, targeting the initializer expression of constants that have slice types within compound types. This applies when a constant's type involves a slice (e.g., &MySlice<[T]>) and the initializer uses a slice literal or array coercion. The mutation changes the array length in the slice data, which may expose layout or scalar representation bugs during const-eval. Only introduce new array elements that are default values of the element type (like false for bool, 0 for integers) to avoid undefined behavior. This operator focuses on altering the slice length within aggregate constants, stressing scalar pair handling."
    }
}