//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Struct_Empty_Return_With_Receiver_217
 */ 
class MutatorFrontendAction_217 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(217)

private:
    class MutatorASTConsumer_217 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_217(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Template_Struct_Empty_Return_With_Receiver_217.h"

// ========================================================================================================
#define MUT217_OUTPUT 1

void MutatorFrontendAction_217::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ReturnStmt>("Return")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto parent = Result.Nodes.getNodeAs<clang::Stmt>("parent");
      if (parent == nullptr)
        return;
      auto func = dyn_cast<CXXMethodDecl>(parent);
      if (func == nullptr)
        return;
      auto ret_type = func->getReturnType();
      if (ret_type->isStructureType() == false)
        return;
      auto struct_decl = ret_type->getAsCXXRecordDecl();
      if (struct_decl == nullptr)
        return;
      if (struct_decl->getDescribedTemplate() == nullptr)
        return;
      auto receiver = func->getThisObjectType();
      if (receiver.isNull())
        return;
      auto receiver_decl = receiver->getAsCXXRecordDecl();
      if (receiver_decl == nullptr)
        return;
      if (receiver_decl != struct_decl)
        return;
      auto receiver_name = receiver_decl->getNameAsString();
      auto replace_content = receiver_name + "{" + "self" + "}";
      llvm::outs() << replace_content << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()),
                          replace_content);
    }
}
  
void MutatorFrontendAction_217::MutatorASTConsumer_217::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = returnStmt(hasParent(stmt().bind("parent"))).bind("Return");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}