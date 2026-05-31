//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_specialization_enable_if_578
 */ 
class MutatorFrontendAction_578 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(578)

private:
    class MutatorASTConsumer_578 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_578(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_specialization_enable_if_578.h"

// ========================================================================================================
#define MUT578_OUTPUT 1

void MutatorFrontendAction_578::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithEnableIf")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
        return;

      auto funcText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
      std::string newSpec = "template<typename T, typename = typename std::enable_if<!std::is_integral<T>::value>::type>";

      size_t pos = funcText.find("template<");
      if (pos != std::string::npos) {
        funcText.insert(pos, newSpec);
      }

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), funcText);
    }
}
  
void MutatorFrontendAction_578::MutatorASTConsumer_578::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasTemplateArgument(0, refersToType(hasDeclaration(namedDecl(hasName("enable_if")))))).bind("FunctionWithEnableIf");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}