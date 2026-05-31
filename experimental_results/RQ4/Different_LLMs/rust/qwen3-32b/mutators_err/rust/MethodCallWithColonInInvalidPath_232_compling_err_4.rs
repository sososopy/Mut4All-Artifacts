use syn::parse_quote;
use crate::mutator::Mutator;
use std::collections::HashSet;
use syn::Ident;
use rand::thread_rng;
use syn::visit_mut::VisitMut;
use syn::ExprCall;
use syn::Expr;
use syn::visit_mut;
use syn::visit::Visit;
use regex::Regex;

pub struct MethodCallWithColonInInvalidPath_232;

impl Mutator for MethodCallWithColonInInvalidPath_232 {
    fn name(&self) -> &str {
        "MethodCallWithColonInInvalidPath_232"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = CallMutator {
            identifiers: HashSet::new(),
            tokens: Vec::new(),
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets method calls on path expressions (e.g., `Type::method(...)`) and transforms them into invalid syntax by replacing the method name with a parenthesized identifier followed by a colon and a token from the seed program (e.g., `Type::(x: !)(...)`). This mimics an invalid colon in a path segment, potentially exposing parser ambiguities or ICEs in the compiler's handling of method call syntax."
    }
}

struct CallMutator {
    identifiers: HashSet<Ident>,
    tokens: Vec<String>,
}

impl CallMutator {
    fn should_mutate(&self) -> bool {
        thread_rng().gen_bool(0.3)
    }

    fn get_random_identifier(&self) -> Option<&Ident> {
        if self.identifiers.is_empty() {
            return None;
        }
        let mut rng = thread_rng();
        let index = rng.gen_range(0..self.identifiers.len());
        self.identifiers.iter().nth(index)
    }

    fn get_random_token(&self) -> Option<&str> {
        if self.tokens.is_empty() {
            return None;
        }
        let mut rng = thread_rng();
        let index = rng.gen_range(0..self.tokens.len());
        self.tokens.get(index).map(|x| x.as_str())
    }
}

impl VisitMut for CallMutator {
    fn visit_expr_call_mut(&mut self, call: &mut ExprCall) {
        if let Expr::Path(expr_path) = &*call.func {
            let path = &expr_path.path;
            if path.segments.len() == 1 {
                if self.should_mutate() {
                    let mut visitor = IdentCollector {
                        identifiers: HashSet::new(),
                    };
                    visitor.visit_path(path);
                    self.identifiers = visitor.identifiers;
                    let token_visitor = TokenCollector {};
                    let source = match path.span().source_text() {
                        Some(s) => s,
                        None => return,
                    };
                    self.tokens = token_visitor.collect_tokens(&source);

                    if let Some(id) = self.get_random_identifier() {
                        if let Some(token_str) = self.get_random_token() {
                            let new_method = parse_quote! {
                                (#id: #token_str)
                            };
                            let new_call = parse_quote! {
                                #path::#new_method(#call.args)
                            };
                            *call = new_call;
                        }
                    }
                }
            }
        }
        visit_mut::visit_expr_call_mut(self, call);
    }
}

struct IdentCollector {
    identifiers: HashSet<Ident>,
}

impl Visit<'_> for IdentCollector {
    fn visit_ident(&mut self, ident: &Ident) {
        self.identifiers.insert(ident.clone());
    }
}

struct TokenCollector {}

impl TokenCollector {
    fn collect_tokens(&self, source: &str) -> Vec<String> {
        let re = Regex::new(r#"[!@#$%^&*()+=\{\}\[\]:;"',<>.?/|\\~`-]"#).unwrap();
        re.find_iter(source)
            .map(|m| source[m.start()..m.end()].to_string())
            .collect()
    }
}