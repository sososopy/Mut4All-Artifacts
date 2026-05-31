//header file
#pragma once
#include "Mutator_base.h"

/**
 * access_empty_parameter_pack_9
 */ 
class MutatorFrontendAction_9 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(9)

private:
    class MutatorASTConsumer_9 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_9(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/access_empty_parameter_pack_9.h"

// ========================================================================================================
#define MUT9_OUTPUT 1

void MutatorFrontendAction_9::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;

      if (FT->getTemplateParameters()->size() == 1) {
        auto paramPack = dyn_cast<clang::TemplateTypeParmDecl>(FT->getTemplateParameters()->getParam(0));
        if (paramPack && paramPack->isParameterPack()) {
          auto funcDecl = FT->getTemplatedDecl();
          if (funcDecl && funcDecl->hasBody()) {
            auto body = funcDecl->getBody();
            std::string bodyStr = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());
            std::string mutation = "anotherFunction<Args...[0]>();";
            bodyStr.insert(bodyStr.find("{") + 1, "\n/*mut9*/" + mutation + "\n");
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyStr);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_9::MutatorASTConsumer_9::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(
        has(templateParameterList(
            has(templateTypeParmDecl(hasTypeParmKind(TPK_Pack)))
        ))
    ).bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}