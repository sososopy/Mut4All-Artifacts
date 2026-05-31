//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Auto-based_Template_Parameter_473
 */ 
class MutatorFrontendAction_473 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(473)

private:
    class MutatorASTConsumer_473 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_473(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Auto-based_Template_Parameter_473.h"

// ========================================================================================================
#define MUT473_OUTPUT 1

void MutatorFrontendAction_473::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("AutoTemplate")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      auto name = TD->getNameAsString();
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             TD->getSourceRange());
      llvm::outs() << content << '\n';
      auto pos = content.find(name);
      if (pos == string::npos)
        return;
      content.insert(pos, "/*mut473*/");
      content += " struct { void operator()(int); } f; ";
      content += name + "<f(T::x)>;";
      llvm::outs() << content << '\n';
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(TD->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_473::MutatorASTConsumer_473::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl(has(templateTypeParmDecl(hasType(qualType(hasDescendant(autoType())))))).bind("AutoTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}