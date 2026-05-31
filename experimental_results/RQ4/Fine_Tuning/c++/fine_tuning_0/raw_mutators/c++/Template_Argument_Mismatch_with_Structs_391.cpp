//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Argument_Mismatch_with_Structs_391
 */ 
class MutatorFrontendAction_391 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(391)

private:
    class MutatorASTConsumer_391 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_391(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Argument_Mismatch_with_Structs_391.h"

// ========================================================================================================
#define MUT391_OUTPUT 1

void MutatorFrontendAction_391::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Template")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      //Get the source code text of target node
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      llvm::outs() << content << '\n';
      content += "/*mut391*/\nstruct X {};\nstruct Y { constexpr Y(const X &) "
                   "{} };\ntemplate <typename T> struct B { constexpr B(){}; "
                   "constexpr B(const T &) {} };\ntemplate <typename T> struct "
                   "C { constexpr C(){}; constexpr C(const T &) {} };\ntemplate "
                   "struct A<B>;\ntemplate struct A<C>;";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_391::MutatorASTConsumer_391::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = classTemplateDecl().bind("Template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}