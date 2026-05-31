//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Auto_With_Explicit_Template_Argument_428
 */ 
class MutatorFrontendAction_428 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(428)

private:
    class MutatorASTConsumer_428 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_428(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Auto_With_Explicit_Template_Argument_428.h"

// ========================================================================================================
#define MUT428_OUTPUT 1

void MutatorFrontendAction_428::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::VarDecl>("AutoVar")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto decl = stringutils::rangetoStr(*(Result.SourceManager),
                                          MT->getSourceRange());
      llvm::outs() << decl << '\n';
      //Perform mutation on the source code text by applying string replacement
      if (MT->getType()->isUndeducedAutoType()) {
        auto type = MT->getType();
        auto deduced_type = MT->getASTContext().getDeducedType(type);
        auto deduced_type_str = deduced_type.getAsString();
        llvm::outs() << deduced_type_str << '\n';
        decl = stringutils::replace_first(decl, "auto", deduced_type_str);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), decl);
    }
}
  
void MutatorFrontendAction_428::MutatorASTConsumer_428::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(autoType())).bind("AutoVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}