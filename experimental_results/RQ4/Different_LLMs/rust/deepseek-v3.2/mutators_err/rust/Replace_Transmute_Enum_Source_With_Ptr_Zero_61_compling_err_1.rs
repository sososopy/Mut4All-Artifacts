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

pub struct Replace_Transmute_Enum_Source_With_Ptr_Zero_61;

impl Mutator for Replace_Transmute_Enum_Source_With_Ptr_Zero_61 {
    fn name(&self) -> &str {
        "Replace_Transmute_Enum_Source_With_Ptr_Zero_61"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut static_items = Vec::new();
        let mut static_names = HashSet::new();
        let mut rng = thread_rng();

        for item in &mut file.items {
            if let Item::Static(item_static) = item {
                static_names.insert(item_static.ident.to_string());
            }
        }

        struct TransmuteVisitor<'a> {
            transmutes: Vec<(Span, Box<Expr>, Box<Type>)>,
            static_items: &'a mut Vec<Item>,
            static_names: &'a HashSet<String>,
            rng: &'a mut rand::rngs::ThreadRng,
        }

        impl<'a> Visit for TransmuteVisitor<'a> {
            fn visit_expr(&mut self, expr: &Expr) {
                if let Expr::Call(call) = expr {
                    if let Expr::Path(path) = &*call.func {
                        if path.path.segments.last().map(|seg| seg.ident.to_string()) == Some("transmute") {
                            if call.args.len() == 1 {
                                let arg = &call.args[0];
                                let dest_type = self.infer_transmute_dest_type(&call.func);
                                if dest_type.is_some() {
                                    self.transmutes.push((expr.span(), Box::new(arg.clone()), dest_type.unwrap()));
                                }
                            }
                        }
                    }
                }
                syn::visit::visit_expr(self, expr);
            }
        }

        impl<'a> TransmuteVisitor<'a> {
            fn infer_transmute_dest_type(&self, func: &Box<Expr>) -> Option<Box<Type>> {
                if let Expr::Path(path) = &**func {
                    if let Some(last_seg) = path.path.segments.last() {
                        if let PathArguments::AngleBracketed(args) = &last_seg.arguments {
                            if args.args.len() == 1 {
                                if let GenericArgument::Type(ty) = &args.args[0] {
                                    return Some(Box::new(ty.clone()));
                                }
                            }
                        }
                    }
                }
                None
            }
        }

        let mut visitor = TransmuteVisitor {
            transmutes: Vec::new(),
            static_items: &mut static_items,
            static_names: &static_names,
            rng: &mut rng,
        };
        visitor.visit_file(file);

        for (span, source_expr, dest_type) in visitor.transmutes {
            if self.is_enum_or_wrapped_enum(&dest_type) {
                let new_source = self.generate_ptr_zero_source(&source_expr, &mut static_items, &mut static_names, &mut rng);
                self.replace_expr_at_span(file, span, new_source);
            }
        }

        file.items.extend(static_items);
    }

    fn is_enum_or_wrapped_enum(&self, ty: &Box<Type>) -> bool {
        match &**ty {
            Type::Path(type_path) => {
                let path_str = type_path.path.segments.iter().map(|seg| seg.ident.to_string()).collect::<Vec<_>>().join("::");
                path_str.contains("Enum") || self.is_generic_wrapping_enum(&type_path.path)
            }
            _ => false,
        }
    }

    fn is_generic_wrapping_enum(&self, path: &SynPath) -> bool {
        for seg in &path.segments {
            if let PathArguments::AngleBracketed(args) = &seg.arguments {
                for arg in &args.args {
                    if let GenericArgument::Type(ty) = arg {
                        if self.is_enum_or_wrapped_enum(&Box::new(ty.clone())) {
                            return true;
                        }
                    }
                }
            }
        }
        false
    }

    fn generate_ptr_zero_source(&self, original_source: &Box<Expr>, static_items: &mut Vec<Item>, static_names: &mut HashSet<String>, rng: &mut rand::rngs::ThreadRng) -> Expr {
        let integer_types = ["i8", "i16", "i32", "i64", "isize", "u8", "u16", "u32", "u64", "usize"];
        let chosen_type = integer_types.choose(rng).unwrap_or(&"i32");

        let static_name = format!("INT_ZERO_{}", chosen_type);
        if !static_names.contains(&static_name) {
            let static_item: Item = parse_quote! {
                static #static_name: #chosen_type = 0;
            };
            static_items.push(static_item);
            static_names.insert(static_name.clone());
        }

        parse_quote! {
            &#static_name
        }
    }

    fn replace_expr_at_span(&self, file: &mut File, span: Span, new_expr: Expr) {
        struct ReplaceVisitor {
            target_span: Span,
            new_expr: Expr,
            replaced: bool,
        }

        impl VisitMut for ReplaceVisitor {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if expr.span() == self.target_span && !self.replaced {
                    *expr = self.new_expr.clone();
                    self.replaced = true;
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }

        let mut visitor = ReplaceVisitor {
            target_span: span,
            new_expr,
            replaced: false,
        };
        visitor.visit_file_mut(file);
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets unsafe transmute expressions where the destination type is an enum or generic wrapping an enum. It replaces the source operand (typically an integer literal) with a reference to a zero of a randomly chosen integer type, stored in a static variable. This transformation creates a pointer value that may be invalid as a discriminant for enums with non-zero niches, potentially triggering layout computation issues, invalid memory accesses, or type confusion in the compiler's transmute validation logic."
    }
}