use proc_macro2::Span;
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

pub struct Replace_Const_Generic_With_Function_Pointer_385;

impl Mutator for Replace_Const_Generic_With_Function_Pointer_385 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_Function_Pointer_385"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut modified_info = Vec::new();

        for item in &mut file.items {
            if let Item::Struct(s) = item {
                if let Some((idx, _)) = find_first_const_generic(&s.generics) {
                    let param = &mut s.generics.params[idx];
                    if let GenericParam::Const(c) = param {
                        let ty = &c.ty;
                        if is_integer_or_bool(ty) {
                            c.ty = syn::parse_quote!(fn(usize) -> bool);
                            modified_info.push(ModifyInfo {
                                name: s.ident.clone(),
                                param_index: idx,
                            });
                        }
                    }
                }
            } else if let Item::Enum(e) = item {
                if let Some((idx, _)) = find_first_const_generic(&e.generics) {
                    let param = &mut e.generics.params[idx];
                    if let GenericParam::Const(c) = param {
                        let ty = &c.ty;
                        if is_integer_or_bool(ty) {
                            c.ty = syn::parse_quote!(fn(usize) -> bool);
                            modified_info.push(ModifyInfo {
                                name: e.ident.clone(),
                                param_index: idx,
                            });
                        }
                    }
                }
            }
        }

        let has_dummy = file.items.iter().any(|item| {
            if let Item::Fn(f) = item {
                f.sig.ident == "dummy"
            } else {
                false
            }
        });

        if !has_dummy {
            let dummy_fn = parse_quote! {
                fn dummy(val: usize) -> bool { true }
            };
            file.items.insert(0, Item::Fn(dummy_fn));
        }

        let mut visitor = ModifyUsagesVisitor {
            modified_info,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const generic parameters in structs/enums with function pointer types and modifies all usages to pass a dummy function. This tests the compiler's handling of const generics with function pointers and ensures that const parameter values are valid expressions."
    }
}

fn find_first_const_generic(generics: &syn::Generics) -> Option<(usize, &syn::GenericParam)> {
    for (i, param) in generics.params.iter().enumerate() {
        if let syn::GenericParam::Const(_) = param {
            return Some((i, param));
        }
    }
    None
}

fn is_integer_or_bool(ty: &syn::Type) -> bool {
    if let syn::Type::Path(ty_path) = ty {
        if let Some(segment) = ty_path.path.segments.last() {
            match segment.ident.to_string().as_str() {
                "usize" | "isize" | "u8" | "u16" | "u32" | "u64" | "u128" | "i8" | "i16" | "i32" | "i64" | "i128" | "bool" => return true,
                _ => {}
            }
        }
    }
    false
}

struct ModifyUsagesVisitor {
    modified_info: Vec<ModifyInfo>,
}

struct ModifyInfo {
    name: syn::Ident,
    param_index: usize,
}

impl ModifyUsagesVisitor {
    fn should_modify(&self, path: &syn::Path) -> Option<usize> {
        if let Some(segment) = path.segments.last() {
            if let Some(info) = self.modified_info.iter().find(|info| info.name == segment.ident) {
                return Some(info.param_index);
            }
        }
        None
    }
}

impl VisitMut for ModifyUsagesVisitor {
    fn visit_expr_struct_mut(&mut self, expr: &mut syn::ExprStruct) {
        self.visit_attributes_mut(&mut expr.attrs);
        if let Some(qself) = &mut expr.qself {
            self.visit_qself_mut(qself);
        }
        self.visit_path_mut(&mut expr.path);
        for field in &mut expr.fields {
            self.visit_expr_mut(&mut field.expr);
        }
        if let Some(rest) = &mut expr.rest {
            self.visit_expr_mut(rest);
        }

        if let Some(param_index) = self.should_modify(&expr.path) {
            if let Some(segment) = expr.path.segments.last_mut() {
                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                    for (i, arg) in args.args.iter_mut().enumerate() {
                        if let GenericArgument::Const(expr) = arg {
                            if i == param_index {
                                *expr = syn::parse_quote!(dummy);
                            }
                        }
                    }
                }
            }
        }
    }

    fn visit_type_path_mut(&mut self, ty: &mut syn::TypePath) {
        if let Some(qself) = &mut ty.qself {
            self.visit_qself_mut(qself);
        }
        self.visit_path_mut(&mut ty.path);

        if let Some(param_index) = self.should_modify(&ty.path) {
            if let Some(segment) = ty.path.segments.last_mut() {
                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                    for (i, arg) in args.args.iter_mut().enumerate() {
                        if let GenericArgument::Const(expr) = arg {
                            if i == param_index {
                                *expr = syn::parse_quote!(dummy);
                            }
                        }
                    }
                }
            }
        }
    }
}