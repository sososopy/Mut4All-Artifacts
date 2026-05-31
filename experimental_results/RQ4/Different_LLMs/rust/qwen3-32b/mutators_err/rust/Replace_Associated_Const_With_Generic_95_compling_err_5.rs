use syn::parse_quote;
use syn::GenericParam;
use syn::TypeParamBound;
use syn::TraitBound;
use syn::PathArguments;
use syn::GenericArgument;
use syn::Expr;
use crate::mutator::Mutator;

pub struct Replace_Associated_Const_With_Generic_95;

impl Mutator for Replace_Associated_Const_With_Generic_95 {
    fn name(&self) -> &str {
        "Replace_Associated_Const_With_Generic_95"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                self.process_function_generics(&mut func.sig.generics);
            } else if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        self.process_function_generics(&mut method.sig.generics);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies trait bounds with associated const literals and replaces them with generic const expressions using the function's const parameters. If no const parameters exist, it adds one. This transformation stresses the compiler's handling of const generics in trait bounds, potentially exposing bugs in const parameter resolution and trait bound validation."
    }
}

impl Replace_Associated_Const_With_Generic_95 {
    fn process_function_generics(&self, generics: &mut syn::Generics) {
        let has_associated_const_literal = self.has_associated_const_literal_generics(generics) || self.has_associated_const_literal_where_clause(generics.where_clause.as_ref());
        if !has_associated_const_literal {
            return;
        }

        let mut const_params: Vec<&GenericParam> = generics.params.iter().filter(|p| matches!(p, GenericParam::Const(_))).collect();
        
        if const_params.is_empty() {
            let new_const = parse_quote! { const N: usize };
            generics.params.push(new_const);
            const_params = generics.params.iter().filter(|p| matches!(p, GenericParam::Const(_))).collect();
        }

        self.replace_in_generics(generics, const_params[0]);

        if let Some(where_clause) = generics.where_clause.as_mut() {
            self.replace_in_where_clause(where_clause, const_params[0]);
        }
    }

    fn has_associated_const_literal_generics(&self, generics: &syn::Generics) -> bool {
        for param in &generics.params {
            if let GenericParam::Type(type_param) = param {
                for bound in &type_param.bounds {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        if self.has_associated_const_literal_in_trait_bound(trait_bound) {
                            return true;
                        }
                    }
                }
            }
        }
        false
    }

    fn has_associated_const_literal_where_clause(&self, where_clause: Option<&syn::WhereClause>) -> bool {
        if let Some(where_clause) = where_clause {
            for predicate in &where_clause.predicates {
                if let syn::WherePredicate::Type(predicate_type) = predicate {
                    for bound in &predicate_type.bounds {
                        if let TypeParamBound::Trait(trait_bound) = bound {
                            if self.has_associated_const_literal_in_trait_bound(trait_bound) {
                                return true;
                            }
                        }
                    }
                }
            }
        }
        false
    }

    fn has_associated_const_literal_in_trait_bound(&self, trait_bound: &TraitBound) -> bool {
        let path = &trait_bound.path;
        if let Some(last_segment) = path.segments.last() {
            if let PathArguments::AngleBracketed(args) = &last_segment.arguments {
                for arg in &args.args {
                    if let GenericArgument::Const(expr) = arg {
                        if self.is_literal_expr(expr) {
                            return true;
                        }
                    }
                }
            }
        }
        false
    }

    fn is_literal_expr(&self, expr: &Expr) -> bool {
        matches!(expr, Expr::Lit(_))
    }

    fn replace_in_generics(&self, generics: &mut syn::Generics, const_param: &GenericParam) {
        let const_param_name = if let GenericParam::Const(const_param) = const_param {
            &const_param.ident
        } else {
            return;
        };

        for param in &mut generics.params {
            if let GenericParam::Type(type_param) = param {
                for bound in &mut type_param.bounds {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        if let Some(last_segment) = trait_bound.path.segments.last_mut() {
                            if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                                for arg in &mut args.args {
                                    if let GenericArgument::Const(expr) = arg {
                                        if self.is_literal_expr(expr) {
                                            let new_expr = parse_quote! { #const_param_name };
                                            *expr = new_expr;
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

    fn replace_in_where_clause(&self, where_clause: &mut syn::WhereClause, const_param: &GenericParam) {
        let const_param_name = if let GenericParam::Const(const_param) = const_param {
            &const_param.ident
        } else {
            return;
        };

        for predicate in &mut where_clause.predicates {
            if let syn::WherePredicate::Type(predicate_type) = predicate {
                for bound in &mut predicate_type.bounds {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        if let Some(last_segment) = trait_bound.path.segments.last_mut() {
                            if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                                for arg in &mut args.args {
                                    if let GenericArgument::Const(expr) = arg {
                                        if self.is_literal_expr(expr) {
                                            let new_expr = parse_quote! { #const_param_name };
                                            *expr = new_expr;
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