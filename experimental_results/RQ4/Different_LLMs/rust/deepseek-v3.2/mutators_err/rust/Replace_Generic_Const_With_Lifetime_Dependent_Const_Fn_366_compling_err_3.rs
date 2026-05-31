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

pub struct Replace_Generic_Const_With_Lifetime_Dependent_Const_Fn_366;

impl Mutator for Replace_Generic_Const_With_Lifetime_Dependent_Const_Fn_366 {
    fn name(&self) -> &str {
        "Replace_Generic_Const_With_Lifetime_Dependent_Const_Fn_366"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut array_replacements = Vec::new();
        let mut visitor = ArrayLengthVisitor {
            replacements: &mut array_replacements,
        };
        visitor.visit_file_mut(file);
        
        for replacement in array_replacements {
            let enclosing_fn = replacement.enclosing_fn;
            let length_expr = replacement.length_expr;
            let array_span = replacement.array_span;
            let lifetime_to_use = replacement.lifetime_to_use;
            
            let inner_fn_name = Ident::new(&format!("inner_{}", replacement.counter), array_span);
            
            let inner_fn: Item = parse_quote! {
                const fn #inner_fn_name<'a>() -> usize where &'a (): Sized { #length_expr }
            };
            
            let call_expr: Expr = parse_quote! {
                #inner_fn_name::<#lifetime_to_use>()
            };
            
            match enclosing_fn {
                Some(mut fn_item) => {
                    if let Item::Fn(func) = &mut *fn_item {
                        func.block.stmts.insert(0, parse_quote!(#inner_fn));
                    }
                }
                None => {
                    file.items.insert(0, inner_fn);
                }
            }
            
            let mut replacer = ArrayLengthReplacer {
                target_span: array_span,
                replacement: call_expr,
                found: false,
            };
            replacer.visit_file_mut(file);
        }
        
        let mut feature_adder = FeatureAdder;
        feature_adder.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets array type annotations and array repeat expressions with generic const expressions. It replaces the length expression with a call to a newly introduced const function that has a generic lifetime parameter and a higher-ranked trait bound (where &'a (): Sized). This mimics the bug-triggering structure that causes ICEs in the compiler's const evaluation when dealing with ReFree regions. The operator ensures the generic_const_exprs feature is enabled and adds appropriate const functions with lifetime-dependent calls."
    }
}

struct ArrayLengthReplacement {
    enclosing_fn: Option<Box<Item>>,
    length_expr: Box<Expr>,
    array_span: Span,
    lifetime_to_use: Lifetime,
    counter: usize,
}

struct ArrayLengthVisitor<'a> {
    replacements: &'a mut Vec<ArrayLengthReplacement>,
}

impl<'a> VisitMut for ArrayLengthVisitor<'a> {
    fn visit_type_array_mut(&mut self, node: &mut TypeArray) {
        let length_expr = node.len.clone();
        let array_span = node.span();
        
        let mut lifetime_finder = LifetimeFinder {
            lifetimes: Vec::new(),
        };
        lifetime_finder.visit_type_array_mut(node);
        
        let lifetime_to_use = if !lifetime_finder.lifetimes.is_empty() {
            lifetime_finder.lifetimes[0].clone()
        } else {
            Lifetime::new("'static", array_span)
        };
        
        let enclosing_fn = self.find_enclosing_fn();
        
        self.replacements.push(ArrayLengthReplacement {
            enclosing_fn,
            length_expr,
            array_span,
            lifetime_to_use,
            counter: self.replacements.len(),
        });
        
        syn::visit_mut::visit_type_array_mut(self, node);
    }
    
    fn visit_expr_repeat_mut(&mut self, node: &mut ExprRepeat) {
        let length_expr = node.len.clone();
        let array_span = node.span();
        
        let mut lifetime_finder = LifetimeFinder {
            lifetimes: Vec::new(),
        };
        lifetime_finder.visit_expr_repeat_mut(node);
        
        let lifetime_to_use = if !lifetime_finder.lifetimes.is_empty() {
            lifetime_finder.lifetimes[0].clone()
        } else {
            Lifetime::new("'static", array_span)
        };
        
        let enclosing_fn = self.find_enclosing_fn();
        
        self.replacements.push(ArrayLengthReplacement {
            enclosing_fn,
            length_expr,
            array_span,
            lifetime_to_use,
            counter: self.replacements.len(),
        });
        
        syn::visit_mut::visit_expr_repeat_mut(self, node);
    }
}

impl<'a> ArrayLengthVisitor<'a> {
    fn find_enclosing_fn(&mut self) -> Option<Box<Item>> {
        None
    }
}

struct LifetimeFinder {
    lifetimes: Vec<Lifetime>,
}

impl VisitMut for LifetimeFinder {
    fn visit_lifetime_mut(&mut self, node: &mut Lifetime) {
        self.lifetimes.push(node.clone());
        syn::visit_mut::visit_lifetime_mut(self, node);
    }
    
    fn visit_generic_param_mut(&mut self, node: &mut GenericParam) {
        if let GenericParam::Lifetime(lifetime_param) = node {
            self.lifetimes.push(lifetime_param.lifetime.clone());
        }
        syn::visit_mut::visit_generic_param_mut(self, node);
    }
}

struct ArrayLengthReplacer {
    target_span: Span,
    replacement: Expr,
    found: bool,
}

impl VisitMut for ArrayLengthReplacer {
    fn visit_type_array_mut(&mut self, node: &mut TypeArray) {
        if node.span().into() == self.target_span.into() && !self.found {
            *node.len = Box::new(self.replacement.clone());
            self.found = true;
        }
        syn::visit_mut::visit_type_array_mut(self, node);
    }
    
    fn visit_expr_repeat_mut(&mut self, node: &mut ExprRepeat) {
        if node.span().into() == self.target_span.into() && !self.found {
            *node.len = Box::new(self.replacement.clone());
            self.found = true;
        }
        syn::visit_mut::visit_expr_repeat_mut(self, node);
    }
}

struct FeatureAdder;

impl VisitMut for FeatureAdder {
    fn visit_file_mut(&mut self, file: &mut File) {
        let mut has_feature = false;
        for item in &file.items {
            if let Item::Verbatim(tokens) = item {
                let tokens_str = tokens.to_string();
                if tokens_str.contains("#![feature(generic_const_exprs)]") {
                    has_feature = true;
                }
            }
        }
        
        if !has_feature {
            let feature_attr: Attribute = parse_quote! {
                #![feature(generic_const_exprs)]
            };
            let allow_attr: Attribute = parse_quote! {
                #![allow(incomplete_features)]
            };
            file.attrs.insert(0, feature_attr);
            file.attrs.insert(1, allow_attr);
        }
        
        syn::visit_mut::visit_file_mut(self, file);
    }
}