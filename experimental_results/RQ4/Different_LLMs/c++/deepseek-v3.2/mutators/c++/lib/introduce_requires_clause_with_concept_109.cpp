//source file
#include "../include/Introduce_Requires_Clause_With_Concept_109.h"

// ========================================================================================================
#define MUT109_OUTPUT 1

void MutatorFrontendAction_109::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplateDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      // Get the function declaration
      auto *FT = FD->getTemplatedDecl();
      if (!FT || !FT->hasBody())
        return;
      
      // Check if it already has a requires clause
      auto *TPL = FD->getTemplateParameters();
      if (!TPL || TPL->size() == 0)
        return;
      
      // Check if there's already a requires clause
      if (FD->getASTContext().getLangOpts().CPlusPlus20 && FD->getInstantiatedFromMemberTemplate())
        return;
      
      // Get source manager
      SourceManager &SM = Rewrite.getSourceMgr();
      
      // First, ensure is_convertible trait exists
      std::string traitCode;
      if (addedTraits.find("is_convertible") == addedTraits.end()) {
        traitCode = "\ntemplate<typename From, typename To>\nstruct is_convertible {\n    static constexpr bool value = true;\n};\n\ntemplate<typename From, typename To>\nconstexpr bool is_convertible_v = is_convertible<From, To>::value;\n";
        
        // Add the trait before the function template
        SourceLocation insertLoc = FD->getBeginLoc();
        Rewrite.InsertTextBefore(insertLoc, "/*mut109*/" + traitCode);
        addedTraits.insert("is_convertible");
      }
      
      // Ensure convertible_to concept exists
      std::string conceptCode;
      if (addedTraits.find("convertible_to") == addedTraits.end()) {
        conceptCode = "\ntemplate<typename From, typename To>\nconcept convertible_to = is_convertible_v<From, To>;\n";
        
        // Add the concept before the function template
        SourceLocation insertLoc = FD->getBeginLoc();
        Rewrite.InsertTextBefore(insertLoc, "/*mut109*/" + conceptCode);
        addedTraits.insert("convertible_to");
      }
      
      // Analyze the function body to find an operation involving template parameters
      std::string operationExpr;
      Stmt *body = FT->getBody();
      if (body) {
        // Look for binary operations in the body
        if (auto *compound = dyn_cast<CompoundStmt>(body)) {
          for (auto *stmt : compound->body()) {
            if (auto *retSt1 = dyn_cast<ReturnStmt>(stmt)) {
              if (auto *expr = retSt1->getRetValue()) {
                if (auto *binOp = dyn_cast<BinaryOperator>(expr)) {
                  operationExpr = stringutils::rangetoStr(SM, binOp->getSourceRange());
                  break;
                }
              }
            } else if (auto *exprStmt = dyn_cast<Expr>(stmt)) {
              if (auto *binOp = dyn_cast<BinaryOperator>(exprStmt)) {
                  operationExpr = stringutils::rangetoStr(SM, binOp->getSourceRange());
                  break;
                }
            }
          }
        }
      }
      
      if (operationExpr.empty()) {
        // Default to a simple comparison if no binary operation found
        operationExpr = "a == b";
      }
      
      // Build the requires clause
      std::string requiresClause = " requires requires(";
      bool firstParam = true;
      for (unsigned i = 0; i < TPL->size(); ++i) {
        auto *param = TPL->getParam(i);
        if (auto *tparam = dyn_cast<TemplateTypeParmDecl>(param)) {
          if (!firstParam) requiresClause += ", ";
          requiresClause += tparam->getNameAsString() + " a, " + tparam->getNameAsString() + " b";
          firstParam = false;
          // Use the first template type parameter for the constraint
          break;
        }
      }
      
      requiresClause += ") { { " + operationExpr + " } -> convertible_to<bool>; }";
      
      // Insert the requires clause after template parameters
      SourceLocation insertLoc = FD->getTemplateParameters()->getSourceRange().getEnd();
      Rewrite.InsertTextAfterToken(insertLoc, "/*mut109*/" + requiresClause);
    }
}
  
void MutatorFrontendAction_109::MutatorASTConsumer_109::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}