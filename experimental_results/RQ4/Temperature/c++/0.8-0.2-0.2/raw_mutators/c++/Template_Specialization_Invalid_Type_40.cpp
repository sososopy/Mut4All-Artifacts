//header file
#pragma once
#include "Mutator_base.h"

/**
 * template_specialization_invalid_type_40
 */ 
class MutatorFrontendAction_40 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(40)

private:
    class MutatorASTConsumer_40 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_40(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/template_specialization_invalid_type_40.h"

// ========================================================================================================
#define MUT40_OUTPUT 1

void MutatorFrontendAction_40::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      const TemplateSpecializationType *TST = nullptr;
      for (auto *param : FD->parameters()) {
        if (param->getType()->isTemplateSpecializationType()) {
          TST = param->getType()->getAs<TemplateSpecializationType>();
          break;
        }
      }

      if (TST && TST->getArg(0).getKind() == TemplateArgument::Type) {
        QualType specializedType = TST->getArg(0).getAsType();
        std::string specializedTypeStr = specializedType.getAsString();
        
        if (specializedType.isConstQualified()) {
          std::string mutationStr = specializedTypeStr;
          mutationStr.erase(0, 6); // remove 'const ' from the start
          std::string callStr = "func(Y<" + mutationStr + ">());";
          SourceLocation insertLoc = FD->getEndLoc();
          Rewrite.InsertTextAfterToken(insertLoc, "\n/*mut40*/" + callStr);
        }
      }
    }
}
  
void MutatorFrontendAction_40::MutatorASTConsumer_40::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasParameter(0, hasType(templateSpecializationType()))).bind("FuncDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}