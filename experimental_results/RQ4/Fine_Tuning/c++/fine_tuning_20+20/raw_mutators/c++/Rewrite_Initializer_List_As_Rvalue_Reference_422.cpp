//header file
#pragma once
#include "Mutator_base.h"

/**
 * Rewrite_Initializer_List_As_Rvalue_Reference_422
 */ 
class MutatorFrontendAction_422 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(422)

private:
    class MutatorASTConsumer_422 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_422(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Rewrite_Initializer_List_As_Rvalue_Reference_422.h"

// ========================================================================================================
#define MUT422_OUTPUT 1

void MutatorFrontendAction_422::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::DeclRefExpr>("InitList")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      //Get the source code text of target node
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (content.find("initializer_list") != string::npos) {
        content = "static_cast<" + content + "&&>(" + content + ")";
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_422::MutatorASTConsumer_422::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = declRefExpr(to(varDecl(hasType(hasUnqualifiedDesugaredType(
                                 recordType(hasDeclaration(cxxRecordDecl(
                                     hasName("initializer_list")))))))))
                      .bind("InitList");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}