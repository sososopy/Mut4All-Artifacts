//source file
#include "../include/modify_template_constraints_446.h"

// ========================================================================================================
#define MUT446_OUTPUT 1

void MutatorFrontendAction_446::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->getDescribedFunctionTemplate()) {
        const auto *TemplateDecl = FD->getDescribedFunctionTemplate();
        // Assuming we need to get constraints from the template parameters
        for (const auto &Param : *TemplateDecl->getTemplateParameters()) {
          if (const auto *TemplateTypeParm = llvm::dyn_cast<clang::TemplateTypeParmDecl>(Param)) {
            if (TemplateTypeParm->hasTypeConstraint()) {
              const auto *ConstraintExpr = TemplateTypeParm->getTypeConstraint();
              std::string originalConstraint = Lexer::getSourceText(CharSourceRange::getTokenRange(ConstraintExpr->getImmediatelyDeclaredConstraint()->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts()).str();
              
              // Perform mutation by adding an additional concept constraint
              std::string mutatedConstraint = "(" + originalConstraint + " && AnotherConcept<T>)";
              
              // Replace the original constraint with the mutated one
              Rewrite.ReplaceText(ConstraintExpr->getImmediatelyDeclaredConstraint()->getSourceRange(), mutatedConstraint);
              
              // Insert a call to std::format in the function body
              if (FD->hasBody()) {
                Stmt *Body = FD->getBody();
                std::string bodyText = Lexer::getSourceText(CharSourceRange::getTokenRange(Body->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts()).str();
                size_t insertPos = bodyText.find("{") + 1;
                std::string formatCall = "\nauto result = std::format(\"{}\", value);\n";
                bodyText.insert(insertPos, formatCall);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(Body->getSourceRange()), bodyText);
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_446::MutatorASTConsumer_446::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation()).bind("funcDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}