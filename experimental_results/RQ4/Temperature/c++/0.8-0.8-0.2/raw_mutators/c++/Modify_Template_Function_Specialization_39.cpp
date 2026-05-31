//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_function_specialization_39
 */ 
class MutatorFrontendAction_39 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(39)

private:
    class MutatorASTConsumer_39 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_39(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_function_specialization_39.h"

// ========================================================================================================
#define MUT39_OUTPUT 1

void MutatorFrontendAction_39::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->isTemplateInstantiation()) {
        auto parentDecl = dyn_cast<CXXRecordDecl>(FD->getParent());
        if (!parentDecl)
          return;

        auto typeForSubstitution = parentDecl->decls_begin();
        for (; typeForSubstitution != parentDecl->decls_end(); ++typeForSubstitution) {
          if (isa<RecordDecl>(*typeForSubstitution)) {
            break;
          }
        }
      
        if (typeForSubstitution != parentDecl->decls_end()) {
          auto newType = (*typeForSubstitution)->getNameAsString();
          auto functionSource = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());

          if (FD->getTemplateSpecializationArgs()) {
            std::string newTemplateArgs = "<" + newType + ">";
            auto argsStart = functionSource.find('<');
            auto argsEnd = functionSource.find('>');

            if (argsStart != std::string::npos && argsEnd != std::string::npos) {
              functionSource.replace(argsStart, argsEnd - argsStart + 1, newTemplateArgs);
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), functionSource);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_39::MutatorASTConsumer_39::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}