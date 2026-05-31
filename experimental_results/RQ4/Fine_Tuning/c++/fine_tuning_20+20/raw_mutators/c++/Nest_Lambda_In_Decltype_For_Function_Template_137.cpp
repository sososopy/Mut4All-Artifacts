//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nest_Lambda_In_Decltype_For_Function_Template_137
 */ 
class MutatorFrontendAction_137 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(137)

private:
    class MutatorASTConsumer_137 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_137(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Nest_Lambda_In_Decltype_For_Function_Template_137.h"

// ========================================================================================================
#define MUT137_OUTPUT 1

void MutatorFrontendAction_137::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemp")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      auto params = FT->getTemplateParameters();
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             params->getSourceRange());
      auto first_param = params->getParam(0);
      auto first_type = stringutils::rangetoStr(*(Result.SourceManager),
                                                first_param->getSourceRange());
      string lambda = "[&]{return " + first_type + "{};}";
      content += ",/*mut137*/decltype(" + lambda + ')';
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(params->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_137::MutatorASTConsumer_137::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("FuncTemp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}