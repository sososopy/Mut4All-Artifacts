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

pub struct Swap_Allocator_Type_Parameters_3;

impl Mutator for Swap_Allocator_Type_Parameters_3 {
    fn name(&self) -> &str {
        "Swap_Allocator_Type_Parameters_3"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut visitor = SwapAllocatorVisitor { rng: &mut rng };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generic functions, structs, and impls that involve allocator or memory management types (MaybeUninit, raw pointers, Allocator). It identifies definitions with at least two type parameters and swaps their positions, ensuring consistency across the definition body. This transformation stresses compiler assumptions about parameter ordering during monomorphization, layout computation, and trait resolution, potentially triggering ICEs similar to allocator API issues."
    }
}

struct SwapAllocatorVisitor<'a, R: Rng> {
    rng: &'a mut R,
}

impl<'a, R: Rng> VisitMut for SwapAllocatorVisitor<'a, R> {
    fn visit_item_mut(&mut self, i: &mut Item) {
        match i {
            Item::Struct(s) => self.handle_struct(s),
            Item::Fn(f) => self.handle_fn(f),
            Item::Impl(i) => self.handle_impl(i),
            _ => {}
        }
        syn::visit_mut::visit_item_mut(self, i);
    }
}

impl<'a, R: Rng> SwapAllocatorVisitor<'a, R> {
    fn contains_allocator_types(&self, ty: &Type) -> bool {
        struct AllocatorTypeVisitor(bool);
        impl<'ast> Visit<'ast> for AllocatorTypeVisitor {
            fn visit_type(&mut self, ty: &'ast Type) {
                match ty {
                    Type::Path(tp) => {
                        let segs = &tp.path.segments;
                        if let Some(last) = segs.last() {
                            let name = last.ident.to_string();
                            if name.contains("MaybeUninit")
                                || name.contains("Allocator")
                                || name == "*mut"
                                || name == "*const"
                            {
                                self.0 = true;
                            }
                        }
                    }
                    Type::Ptr(tptr) => {
                        self.0 = true;
                        syn::visit::visit_type(self, &tptr.elem);
                    }
                    _ => syn::visit::visit_type(self, ty),
                }
            }
        }
        let mut visitor = AllocatorTypeVisitor(false);
        visitor.visit_type(ty);
        visitor.0
    }

    fn handle_struct(&mut self, s: &mut ItemStruct) {
        let type_params: Vec<_> = s
            .generics
            .params
            .iter()
            .filter_map(|p| match p {
                GenericParam::Type(tp) => Some(tp.ident.clone()),
                _ => None,
            })
            .collect();
        if type_params.len() < 2 {
            return;
        }
        let mut has_allocator = false;
        for field in &s.fields {
            if self.contains_allocator_types(&field.ty) {
                has_allocator = true;
                break;
            }
        }
        if !has_allocator {
            return;
        }
        let idx1 = self.rng.gen_range(0..type_params.len());
        let idx2 = self.rng.gen_range(0..type_params.len());
        if idx1 == idx2 {
            return;
        }
        let param1 = type_params[idx1].clone();
        let param2 = type_params[idx2].clone();
        let mut new_params = s.generics.params.clone();
        for (i, p) in new_params.iter_mut().enumerate() {
            if let GenericParam::Type(tp) = p {
                if tp.ident == param1 {
                    tp.ident = param2.clone();
                } else if tp.ident == param2 {
                    tp.ident = param1.clone();
                }
            }
        }
        s.generics.params = new_params;
        for field in &mut s.fields {
            self.swap_type_params_in_type(&mut field.ty, &param1, &param2);
        }
        if let Some(where_clause) = &mut s.generics.where_clause {
            for predicate in &mut where_clause.predicates {
                match predicate {
                    WherePredicate::Type(pt) => {
                        self.swap_type_params_in_type(&mut pt.bounded_ty, &param1, &param2);
                        for bound in &mut pt.bounds {
                            if let TypeParamBound::Trait(tb) = bound {
                                self.swap_type_params_in_path(&mut tb.path, &param1, &param2);
                            }
                        }
                    }
                    WherePredicate::Lifetime(_) => {}
                    WherePredicate::EqPredicate(pe) => {
                        self.swap_type_params_in_type(&mut pe.lhs_ty, &param1, &param2);
                        self.swap_type_params_in_type(&mut pe.rhs_ty, &param1, &param2);
                    }
                }
            }
        }
    }

    fn handle_fn(&mut self, f: &mut ItemFn) {
        if f.sig.ident == "main" {
            return;
        }
        let type_params: Vec<_> = f
            .sig
            .generics
            .params
            .iter()
            .filter_map(|p| match p {
                GenericParam::Type(tp) => Some(tp.ident.clone()),
                _ => None,
            })
            .collect();
        if type_params.len() < 2 {
            return;
        }
        let mut has_allocator = false;
        for input in &f.sig.inputs {
            if let FnArg::Typed(pt) = input {
                if self.contains_allocator_types(&pt.ty) {
                    has_allocator = true;
                    break;
                }
            }
        }
        if let ReturnType::Type(_, ty) = &f.sig.output {
            if self.contains_allocator_types(ty) {
                has_allocator = true;
            }
        }
        if !has_allocator {
            return;
        }
        let idx1 = self.rng.gen_range(0..type_params.len());
        let idx2 = self.rng.gen_range(0..type_params.len());
        if idx1 == idx2 {
            return;
        }
        let param1 = type_params[idx1].clone();
        let param2 = type_params[idx2].clone();
        let mut new_params = f.sig.generics.params.clone();
        for (i, p) in new_params.iter_mut().enumerate() {
            if let GenericParam::Type(tp) = p {
                if tp.ident == param1 {
                    tp.ident = param2.clone();
                } else if tp.ident == param2 {
                    tp.ident = param1.clone();
                }
            }
        }
        f.sig.generics.params = new_params;
        for input in &mut f.sig.inputs {
            if let FnArg::Typed(pt) = input {
                self.swap_type_params_in_type(&mut pt.ty, &param1, &param2);
            }
        }
        if let ReturnType::Type(_, ty) = &mut f.sig.output {
            self.swap_type_params_in_type(ty, &param1, &param2);
        }
        if let Some(where_clause) = &mut f.sig.generics.where_clause {
            for predicate in &mut where_clause.predicates {
                match predicate {
                    WherePredicate::Type(pt) => {
                        self.swap_type_params2_in_type(&mut pt.bounded_ty, &param1, &param2);
                        for bound in &mut pt.bounds {
                            if let TypeParamBound::Trait(tb) = bound {
                                self.swap_type_params_in_path(&mut tb.path, &param1, &param2);
                            }
                        }
                    }
                    WherePredicate::Lifetime(_) => {}
                    WherePredicate::EqPredicate(pe) => {
                        self.swap_type_params_in_type(&mut pe.lhs_ty, &param1, &param2);
                        self.swap_type_params_in_type(&mut pe.rhs_ty, &param1, &param2);
                    }
                }
            }
        }
        self.visit_block_mut(&mut f.block);
    }

    fn handle_impl(&mut self, i: &mut ItemImpl) {
        let type_params: Vec<_> = i
            .generics
            .params
            .iter()
            .filter_map(|p| match p {
                GenericParam::Type(tp) => Some(tp.ident.clone()),
                _ => None,
            })
            .collect();
        if type_params.len() < 2 {
            return;
        }
        let mut has_allocator = false;
        if let Type::Path(tp) = &*i.self_ty {
            if self.contains_allocator_types(&Type::Path(tp.clone())) {
                has_allocator = true;
            }
        }
        if !has_allocator {
            for item in &i.items {
                if let ImplItem::Fn(method) = item {
                    for input in &method.sig.inputs {
                        if let FnArg::Typed(pt) = input {
                            if self.contains_allocator_types(&pt.ty) {
                                has_allocator = true;
                                break;
                            }
                        }
                    }
                    if let ReturnType::Type(_, ty) = &method.sig.output {
                        if self.contains_allocator_types(ty) {
                            has_allocator = true;
                        }
                    }
                }
            }
        }
        if !has_allocator {
            return;
        }
        let idx1 = self.rng.gen_range(0..type_params.len());
        let idx2 = self.rng.gen_range(0..type_params.len());
        if idx1 == idx2 {
            return;
        }
        let param1 = type_params[idx1].clone();
        let param2 = type_params[idx2].clone();
        let mut new_params = i.generics.params.clone();
        for (i, p) in new_params.iter_mut().enumerate() {
            if let GenericParam::Type(tp) = p {
                if tp.ident == param1 {
                    tp.ident = param2.clone();
                } else if tp.ident == param2 {
                    tp.ident = param1.clone();
                }
            }
        }
        i.generics.params = new_params;
        self.swap_type_params_in_type(&mut i.self_ty, &param1, &param2);
        if let Some((_, path, _)) = &mut i.trait_ {
            self.swap_type_params_in_path(path, &param1, &param2);
        }
        if let Some(where_clause) = &mut i.generics.where_clause {
            for predicate in &mut where_clause.predicates {
                match predicate {
                    WherePredicate::Type(pt) => {
                        self.swap_type_params_in_type(&mut pt.bounded_ty, &param1, &param2);
                        for bound in &mut pt.bounds {
                            if let TypeParamBound::Trait(tb) = bound {
                                self.swap_type_params_in_path(&mut tb.path, &param1, &param2);
                            }
                        }
                    }
                    WherePredicate::Lifetime(_) => {}
                    WherePredicate::EqPredicate(pe) => {
                        self.swap_type_params_in_type(&mut pe.lhs_ty, &param1, &param2);
                        self.swap_type_params_in_type(&mut pe.rhs_ty, &param1, &param2);
                    }
                }
            }
        }
        for item in &mut i.items {
            if let ImplItem::Fn(method) = item {
                let mut item_fn = ItemFn {
                    attrs: method.attrs.clone(),
                    vis: method.vis.clone(),
                    sig: method.sig.clone(),
                    block: method.block.clone(),
                };
                self.handle_fn(&mut item_fn);
                method.sig = item_fn.sig;
                method.block = item_fn.block;
            }
        }
    }

    fn swap_type_params_in_type(&mut self, ty: &mut Type, param1: &Ident, param2: &Ident) {
        match ty {
            Type::Path(tp) => {
                self.swap_type_params_in_path(&mut tp.path, param1, param2);
            }
            Type::Ptr(tptr) => {
                self.swap_type_params_in_type(&mut tptr.elem, param1, param2);
            }
            Type::Array(arr) => {
                self.swap_type_params_in_type(&mut arr.elem, param1, param2);
            }
            Type::Slice(slice) => {
                self.swap_type_params_in_type(&mut slice.elem, param1, param2);
            }
            Type::Tuple(tuple) => {
                for elem in &mut tuple.elems {
                    self.swap_type_params_in_type(elem, param1, param2);
                }
            }
            Type::Reference(tref) => {
                self.swap_type_params_in_type(&mut tref.elem, param1, param2);
            }
            Type::BareFn(bfn) => {
                for arg in &mut bfn.inputs {
                    self.swap_type_params_in_type(&mut arg.ty, param1, param2);
                }
                if let ReturnType::Type(_, ret_ty) = &mut bfn.output {
                    self.swap_type_params_in_type(ret_ty, param1, param2);
                }
            }
            Type::ImplTrait(timpl) => {
                for bound in &mut timpl.bounds {
                    if let TypeParamBound::Trait(tb) = bound {
                        self.swap_type_params_in_path(&mut tb.path, param1, param2);
                    }
                }
            }
            Type::TraitObject(tobj) => {
                for bound in &mut tobj.bounds {
                    if let TypeParamBound::Trait(tb) = bound {
                        self.swap_type_params_in_path(&mut tb.path, param1, param2);
                    }
                }
            }
            _ => {}
2        }
    }

    fn swap_type_params_in_path(&mut self, path: &mut SynPath, param1: &Ident, param2: &Ident) {
        for segment in &mut path.segments {
            match &mut segment.arguments {
                PathArguments::AngleBracketed(args) => {
                    for arg in &mut args.args {
                        match arg {
                            GenericArgument::Type(ty) => {
                                self.swap_type_params_in_type(ty, param1, param2);
                            }
                            GenericArgument::AssocType(binding) => {
                                self.swap_type_params_in_type(&mut binding.ty, param1, param2);
                            }
                            _ => {}
                        }
                    }
                }
                PathArguments::Parenthesized(args) => {
                    for input in &mut args.inputs {
                        self.swap_type_params_in_type(input, param1, param2);
                    }
                    if let ReturnType::Type(_, ret_ty) = &mut args.output {
                        self.swap_type_params_in_type(ret_ty, param1, param2);
                    }
                }
                _ => {}
            }
            if segment.ident == *param1 {
                segment.ident = param2.clone();
            } else if segment.ident == *param2 {
                segment.ident = param1.clone();
            }
        }
    }

    fn swap_type_params2_in_type(&mut self, ty: &mut Type, param1: &Ident, param2: &Ident) {
        self.swap_type_params_in_type(ty, param1, param2);
    }
}