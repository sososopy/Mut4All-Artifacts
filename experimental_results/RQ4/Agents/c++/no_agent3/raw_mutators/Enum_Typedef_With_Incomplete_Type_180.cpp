//header file
#pragma once
#include "Mutator_base.h"

/**
 * Enum_Typedef_With_Incomplete_Type_180
 */ 
class MutatorFrontendAction_180 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(180)

private:
    class MutatorASTConsumer_180 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_180(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Enum_Typedef_With_Incomplete_Type_180.h"

// ========================================================================================================
#define MUT180_OUTPUT 1

void MutatorFrontendAction_180::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
      //Filter nodes in header files
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;

      //Get the source code text of target node
      auto enumSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                ED->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string typedefIncomplete = "struct UndefinedType; typedef struct UndefinedType IncompleteAlias;\n";
      std::string mutation = "IncompleteAlias = sizeof(IncompleteAlias)";

      //Replace the original AST node with the mutated one
      Rewrite.InsertTextBefore(ED->getBeginLoc(), "/*mut180*/" + typedefIncomplete);
      Rewrite.ReplaceText(ED->getSourceRange(), enumSource + "\n" + mutation);
    }
}
  
void MutatorFrontendAction_180::MutatorASTConsumer_180::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumDecl().bind("EnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}