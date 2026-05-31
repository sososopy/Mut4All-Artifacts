//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Consteval_Template_In_Malformed_For_306
 */ 
class MutatorFrontendAction_306 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(306)

private:
    class MutatorASTConsumer_306 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_306(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl*> structsWithAlias;
        std::vector<const clang::TypeAliasDecl*> templateAliases;
        std::vector<const clang::CXXRecordDecl*> constevalStructs;
    };
};

//source file
#include "../include/Mutator_Insert_Consteval_Template_In_Malformed_For_306.h"

// ========================================================================================================
#define MUT306_OUTPUT 1

void MutatorFrontendAction_306::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Struct")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      structsWithAlias.push_back(MT);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("Alias")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      templateAliases.push_back(MT);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ConstevalStruct")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      constevalStructs.push_back(MT);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::ForStmt>("MalformedFor")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto *Init = MT->getInit();
      auto *Cond = MT->getCond();
      auto *Inc = MT->getInc();
      //Check if the for-loop is malformed (missing at least one component)
      if ((Init == nullptr && Cond == nullptr) || (Init == nullptr && Inc == nullptr) || (Cond == nullptr && Inc == nullptr)) {
        //Find a suitable struct, alias, and consteval struct from recorded nodes
        if (structsWithAlias.empty() || templateAliases.empty() || constevalStructs.empty())
          return;
        size_t structIdx = getrandom::getRandomIndex(structsWithAlias.size() - 1);
        size_t aliasIdx = getrandom::getRandomIndex(templateAliases.size() - 1);
        size_t constevalIdx = getrandom::getRandomIndex(constevalStructs.size() - 1);
        const clang::CXXRecordDecl* targetStruct = structsWithAlias[structIdx];
        const clang::TypeAliasDecl* targetAlias = templateAliases[aliasIdx];
        const clang::CXXRecordDecl* targetConsteval = constevalStructs[constevalIdx];
        //Generate variable name based on existing names in the context
        std::string varName = "foos";
        //Construct the inserted statement
        std::string aliasName = targetAlias->getNameAsString();
        std::string constevalName = targetConsteval->getNameAsString();
        std::string insertStatement = aliasName + "<> " + varName + " = " + constevalName + "();";
        //Perform mutation on the source code text by applying string replacement
        clang::Stmt* Body = MT->getBody();
        if (Body) {
          SourceLocation insertLoc = Body->getBeginLoc().getLocWithOffset(1);
          Rewrite.InsertText(insertLoc, "\n/*mut306*/" + insertStatement + "\n");
        }
      }
    }
}
  
void MutatorFrontendAction_306::MutatorASTConsumer_306::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher structMatcher = cxxRecordDecl(isStruct()).bind("Struct");
    DeclarationMatcher aliasMatcher = typeAliasDecl().bind("Alias");
    DeclarationMatcher constevalMatcher = cxxRecordDecl(hasAttr(clang::attr::Consteval)).bind("ConstevalStruct");
    StatementMatcher malformedForMatcher = forStmt(anyOf(hasCondition(expr()), hasIncrement(expr()), hasInit(declStmt()))).bind("MalformedFor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(structMatcher, &callback);
    matchFinder.addMatcher(aliasMatcher, &callback);
    matchFinder.addMatcher(constevalMatcher, &callback);
    matchFinder.addMatcher(malformedForMatcher, &callback);
    matchFinder.matchAST(Context);
}