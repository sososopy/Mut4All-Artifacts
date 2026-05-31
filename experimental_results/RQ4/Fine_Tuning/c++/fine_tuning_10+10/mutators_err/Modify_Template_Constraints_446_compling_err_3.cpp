//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Constraints_446
 */ 
class MutatorFrontendAction_446 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(446)

private:
    class MutatorASTConsumer_446 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_446(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
    };
};

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
              const auto *ConstraintExpr = TemplateTypeParm->getTypeConstraintConcept();
              std::string originalConstraint = Lexer::getSourceText(CharSourceRange::getTokenRange(ConstraintExpr->getLocation()), *Result.SourceManager, Result.Context->getLangOpts()).str();
              
              // Perform mutation by adding an additional concept constraint
              std::string mutatedConstraint = "(" + originalConstraint + " && AnotherConcept<T>)";
              
              // Replace the original constraint with the mutated one
              Rewrite.ReplaceText(ConstraintExpr->getLocation(), mutatedConstraint);
              
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