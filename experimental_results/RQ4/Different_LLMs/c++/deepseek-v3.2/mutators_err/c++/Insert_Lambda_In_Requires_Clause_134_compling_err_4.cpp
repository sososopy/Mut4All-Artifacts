//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Lambda_In_Requires_Clause_134
 */ 
class MutatorFrontendAction_134 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(134)

private:
    class MutatorASTConsumer_134 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_134(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT134_OUTPUT 1

void MutatorFrontendAction_134::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructor")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto constructorText = stringutils::rangetoStr(*(Result.SourceManager),
                                                     MT->getSourceRange());
      //Check if constructor has trailing requires clause
      if (!MT->hasTrailingRequiresClause()) {
        //If no trailing requires clause, add one with lambda
        //Find a parameter to capture
        if (MT->getNumParams() == 0) return;
        auto param = MT->getParamDecl(0);
        std::string paramName = param->getNameAsString();
        if (paramName.empty()) return;
        
        //Insert requires clause before body
        std::size_t bodyPos = constructorText.find('{');
        if (bodyPos == std::string::npos) return;
        
        std::string requiresClause = " requires (requires () { [&] -> decltype(" + paramName + ") { return {}; }; }) ";
        constructorText.insert(bodyPos, requires2);
      } else {
        //Has trailing requires clause, insert lambda inside
        auto trailingReq = MT->getTrailingRequiresClause();
        auto reqText = stringutils::rangetoStr(*(Result.SourceManager),
                                               trailingReq->getSourceRange());
        
        //Find a parameter to capture
        if (MT->getNumParams() == 0) return;
        auto param = MT->getParamDecl(0);
        std::string paramName = param->getNameAsString();
        if (paramName.empty()) return;
        
        //Insert lambda inside requires clause
        std::string lambdaExpr = "[&] -> decltype(" + paramName + ") { return {}; }";
        
        //Insert at appropriate position within requires clause
        std::size_t insertPos = reqText.find(')');
        if (insertPos == std::string::npos) return;
        
        std::string newReqText = reqText;
        newReqText.insert(insertPos, " && requires () { " + lambdaExpr