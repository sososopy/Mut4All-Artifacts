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

pub struct Replace_Static_Lifetime_Bound_With_Generic_Lifetime_Parameter_In_Const_Generic_Parameter_Types_144;

impl Mutator for Replace_Static_Lifetime_Bound_With_Generic_Lifetime_Parameter_In_Const_Generic_Parameter_Types_144 {
    fn name(&self) -> &str {
        "Replace_Static_Lifetime_Bound_With_Generic_Lifetime_Parameter_In_Const_Generic_Parameter_Types_144"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct MutatorVisitor<'a> {
            lifetime_name: Option<Lifetime>,
            has_lifetime_param: bool,
            file: &'a mut File,
        }

        impl<'a> MutatorVisitor<'a> {
            fn new(file: &'a mut File) -> Self {
                MutatorVisitor {
                    lifetime_name: None,
                    has_lifetime_param: false,
                    file,
                }
            }

            fn find_or_create_lifetime_param(&mut self, generics: &mut Generics) -> Lifetime {
                if let Some(ref lt) = self.lifetime_name {
                    return lt.clone();
                }

                let new_lifetime = Lifetime::new("'a", Span::call_site());
                self.lifetime_name = Some(new_lifetime.clone());

                // Check if lifetime parameter already exists
                for param in &generics.params {
                    if let GenericParam::Lifetime(lp) = param {
                        if lp.lifetime == new_lifetime {
                            self.has_lifetime_param = true;
                            return new_lifetime.clone();
                        }
                    }
                }

                // Insert lifetime parameter at the beginning of generic parameters
                generics.params.insert(0, GenericParam::Lifetime(LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: new_lifetime.clone(),
                    colon_token: None,
                    bounds: Punctuated::new(),
                }));
                self.has_lifetime_param = true;
                new_lifetime
            }

            fn replace_static_in_type(&self, ty: &mut Type, new_lifetime: &Lifetime) -> bool {
                match ty {
                    Type::Reference(ref_type) => {
                        if let Some(ref lifetime) = ref_type.lifetime {
                            if lifetime.to_string() == "'static" {
                                ref_type.lifetime = Some(new_lifetime.clone());
                                return true;
                            }
                        }
                        false
                    }
                    Type::Slice(slice_type) => {
                        self.replace_static_in_type(&mut slice_type.elem, new_lifetime)
                    }
                    Type::Array(array_type) => {
                        self.replace_static_in_type(&mut array_type.elem, new_lifetime)
                    }
                    Type::Tuple(tuple_type) => {
                        let mut changed = false;
                        for elem in &mut tuple_type.elems {
                            changed |= self.replace_static_in_type(elem, new_lifetime);
                        }
                        changed
                    }
                    Type::Path(type_path) => {
                        let mut changed = false;
                        for segment in &mut type_path.path.segments {
                            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                for arg in &mut args.args {
                                    if let GenericArgument::Type(arg_type) = arg {
                                        changed |= self.replace_static_in_type(arg_type, new_lifetime);
                                    }
                                }
                            }
                        }
                        changed
                    }
                    _ => false,
                }
            }

            fn process_generic_param(&mut self, param: &mut GenericParam, generics: &mut Generics) -> bool {
                if let GenericParam::Const(const_param) = param {
                    let new_lifetime = self.find_or_create_lifetime_param(generics);
                    let mut changed = self.replace_static_in_type(&mut const_param.ty, &new_lifetime);
                    
                    // Also check bounds in const_param.bounds
                    if let Some(bounds) = &mut const_param.bounds {
                        for bound in bounds {
                            if let TypeParamBound::Lifetime(lifetime_bound) = bound {
                                if lifetime_bound.to_string() == "'static" {
                                    *bound = TypeParamBound::Lifetime(new_lifetime.clone());
                                    changed = true;
                                }
                            }
                        }
                    }
                    changed
                } else {
                    false
                }
            }

            fn process_where_clause(&mut self, where_clause: &mut Option<WhereClause>, new_lifetime: &Lifetime) {
                if let Some(clause) = where_clause {
                    for predicate in &mut clause.predicates {
                        match predicate {
                            WherePredicate::Type(pred_type) => {
                                for bound in &mut pred_type.bounds {
                                    if let TypeParamBound::Lifetime(lifetime_bound) = bound {
                                        if lifetime_bound.to_string() == "'static" {
                                            *bound = TypeParamBound::Lifetime(new_lifetime.clone());
                                        }
                                    }
                                }
                            }
                            WherePredicate::Lifetime(pred_lifetime) => {
                                if pred_lifetime.lifetime.to_string() == "'static" {
                                    pred_lifetime.lifetime = new_lifetime.clone();
                                }
                                for bound in &mut pred_lifetime.bounds {
                                    if bound.to_string() == "'static" {
                                        *bound = new_lifetime.clone();
                                    }
                                }
                            }
                            _ => {}
                        }
                    }
                }
            }

            fn process_return_type(&mut self, output: &mut ReturnType, new_lifetime: &Lifetime) {
                if let ReturnType::Type(_, ty) = output {
                    self.replace_static_in_type(&mut **ty, new_lifetime);
                }
            }

            fn process_item(&mut self, item: &mut Item) {
                match item {
                    Item::Fn(item_fn) => {
                        let mut changed = false;
                        let new_lifetime = self.find_or_create_lifetime_param(&mut item_fn.sig.generics);
                        
                        // Process const generic parameters
                        let params = std::mem::take(&mut item_fn.sig.generics.params);
                        for mut param in params {
                            changed |= self.process_generic_param(&mut param, &mut item_fn.sig.generics);
                            item_fn.sig.generics.params.push(param);
                        }
                        
                        if changed {
                            // Process where clause
                            self.process_where_clause(&mut item_fn.sig.generics.where_clause, &new_lifetime);
                            // Process return type
                            self.process_return_type(&mut item_fn.sig.output, &new_lifetime);
                            // Process body - replace static lifetimes in types within the function body
                            self.process_block(&mut item_fn.block, &new_lifetime);
                        }
                    }
                    Item::Struct(item_struct) => {
                        let mut changed = false;
                        let new_lifetime = self.find_or_create_lifetime_param(&mut item_struct.generics);
                        
                        let params = std::mem::take(&mut item_struct.generics.params);
                        for mut param in params {
                            changed |= self.process_generic_param(&mut param, &mut item_struct.generics);
                            item_struct.generics.params.push(param);
                        }
                        
                        if changed {
                            self.process_where_clause(&mut item_struct.generics.where_clause, &new_lifetime);
                            // Process fields
                            for field in &mut item_struct.fields {
                                self.replace_static_in_type(&mut field.ty, &new_lifetime);
                            }
                        }
                    }
                    Item::Impl(item_impl) => {
                        let mut changed = false;
                        let new_lifetime = self.find_or_create_lifetime_param(&mut item_impl.generics);
                        
                        let params = std::mem::take(&mut item_impl.generics.params);
                        for mut param in params {
                            changed |= self.process_generic_param(&mut param, &mut item_impl.generics);
                            item_impl.generics.params.push(param);
                        }
                        
                        if changed {
                            self.process_where_clause(&mut item_impl.generics.where_clause, &new_lifetime);
                            for impl_item in &mut item_impl.items {
                                match impl_item {
                                    ImplItem::Fn(method) => {
                                        let method_lifetime = self.find_or_create_lifetime_param(&mut method.sig.generics);
                                        let method_params = std::mem::take(&mut method.sig.generics.params);
                                        for mut param in method_params {
                                            self.process_generic_param(&mut param, &mut method.sig.generics);
                                            method.sig.generics.params.push(param);
                                        }
                                        self.process_where_clause(&mut method.sig.generics.where_clause, &method_lifetime);
                                        self.process_return_type(&mut method.sig.output, &method_lifetime);
                                        self.process_block(&mut method.block, &method_lifetime);
                                    }
                                    _ => {}
                                }
                            }
                        }
                    }
                    Item::Trait(item_trait) => {
                        let mut changed = false;
                        let new_lifetime = self.find_or_create_lifetime_param(&mut item_trait.generics);
                        
                        let params = std::mem::take(&mut item_trait.generics.params);
                        for mut param in params {
                            changed |= self.process_generic_param(&mut param, &mut item_trait.generics);
                            item_trait.generics.params.push(param);
                        }
                        
                        if changed {
                            self.process_where_clause(&mut item_trait.generics.where_clause, &new_lifetime);
                            for item in &mut item_trait.items {
                                if let TraitItem::Fn(method) = item {
                                    let method_lifetime = self.find_or_create_lifetime_param(&mut method.sig.generics);
                                    let method_params = std::mem::take(&mut method.sig.generics.params);
                                    for mut param in method_params {
                                        self.process_generic_param(&mut param, &mut method.sig.generics);
                                        method.sig.generics.params.push(param);
                                    }
                                    self.process_where_clause(&mut method.sig.generics.where_clause, &method_lifetime);
                                    self.process_return_type(&mut method.sig.output, &method_lifetime);
                                }
                            }
                        }
                    }
                    _ => {}
                }
            }

            fn process_block(&mut self, block: &mut Block, new_lifetime: &Lifetime) {
                for stmt in &mut block.stmts {
                    match stmt {
                        Stmt::Local(local) => {
                            if let Pat::Type(pat_type) = &mut local.pat {
                                self.replace_static_in_type(&mut pat_type.ty, new_lifetime);
                            }
                        }
                        Stmt::Expr(expr, _) => {
                            self.process_expr(expr, new_lifetime);
                        }
                        Stmt::Item(item) => {
                            self.process_item(item);
                        }
                        _ => {}
                    }
                }
            }

            fn process_expr(&mut self, expr: &mut Expr, new_lifetime: &Lifetime) {
                match expr {
                    Expr::Cast(cast_expr) => {
                        self.replace_static_in_type(&mut cast_expr.ty, new_lifetime);
                    }
                    Expr::Type(expr_type) => {
                        self.replace_static_in_type(&mut expr_type.ty, new_lifetime);
                    }
                    Expr::Call(call_expr) => {
                        self.process_expr(&mut call_expr.func, new_lifetime);
                    }
                    Expr::MethodCall(method_call) => {
                        self.process_expr(&mut method_call.receiver, new_lifetime);
                    }
                    Expr::Path(path_expr) => {
                        // Check path segments for generic arguments
                        for segment in &mut path_expr.path.segments {
                            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                for arg in &mut args.args {
                                    if let GenericArgument::Type(arg_type) = arg {
                                        self.replace_static_in_type(arg_type, new_lifetime);
                                    }
                                }
                            }
                        }
                    }
                    _ => {}
                }
            }
        }

        let mut visitor = MutatorVisitor::new(file);
        let items = std::mem::take(&mut visitor.file.items);
        for mut item in items {
            visitor.process_item(&mut item);
            visitor.file.items.push(item);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const generic parameters that are references with explicit static lifetime bounds. It replaces 'static lifetime bounds with a generic lifetime parameter 'a, introducing it if not already present. This transformation applies to functions, structs, impl blocks, and traits, adjusting const generic parameter types, where clauses, return types, and internal type references. The mutation aims to stress the compiler's lifetime handling for const generics, potentially triggering region-related bugs and lifetime inference issues."
    }
}