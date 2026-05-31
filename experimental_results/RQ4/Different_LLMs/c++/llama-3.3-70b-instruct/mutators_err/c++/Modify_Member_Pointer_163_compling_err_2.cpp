//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Member_Pointer_163
 */ 
class MutatorFrontendAction_163 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_163>(TheRewriter);
    }

private:
    class MutatorASTConsumer_163 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_163(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Modify_Member_Pointer_163.h"

// ========================================================================================================
#define MUT163_OUTPUT 1

void MutatorFrontendAction_163::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::UnaryOperator>("UnaryOperator")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocStart()))
        return;
      if (MT->getOpcode() != UnaryOperatorKind::UO_AddrOf) 
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto targetDecl = MT->getSubExpr()->getType().getPointeeType().getCanonicalType().getAsString();
      auto mutatedDecl = targetDecl.substr(0, targetDecl.find("::")) + "::non_existent_member";
      declaration.replace(declaration.find(targetDecl), targetDecl.length(), mutatedDecl);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_163::MutatorASTConsumer_163::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = unaryOperator(hasUnaryOperator(UnaryOperatorKind::UO_AddrOf)).bind("UnaryOperator");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}