//header file
#pragma once
#include "Mutator_base.h"

/**
 * UsingEnumInCompoundExpression_437
 */ 
class MutatorFrontendAction_437 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(437)

private:
    class MutatorASTConsumer_437 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_437(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::EnumDecl*> candidateEnums;
        std::vector<const clang::CXXRecordDecl*> enclosingRecords;
        std::vector<const clang::FunctionDecl*> candidateFunctions;
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT437_OUTPUT 1

void MutatorFrontendAction_437::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
      //Filter nodes in header files
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;
      //Record candidate enum declarations inside struct/class
      if (ED->getDeclContext()->isRecord()) {
        candidateEnums.push_back(ED);
        enclosingRecords.push_back(cast<clang::CXXRecordDecl>(ED->getDeclContext()));
      }
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Record candidate functions (prefer main)
      candidateFunctions.push_back(FD);
    }
    else if (auto *EDR = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDeclRef")) {
      //Filter nodes in header files
      if (!EDR || !Result.Context->getSourceManager().isWrittenInMainFile(
                     EDR->getLocation()))
        return;
      //Check if we have collected enough data to perform mutation
      if (candidateEnums.empty() || candidateFunctions.empty())
        return;
      
      //Choose a random enum from candidates
      size_t enumIdx = getrandom::getRandomIndex(candidateEnums.size() - 1);
      const clang::EnumDecl* targetEnum = candidateEnums[enumIdx];
      const clang::CXXRecordDecl* enclosingRecord = enclosingRecords[enumIdx];
      
      //Choose a function (prefer main if exists)
      const clang::FunctionDecl* targetFunc = nullptr;
      for (auto* func : candidateFunctions) {
        if (func->getNameAsString() == "main") {
          targetFunc = func;
          break;
        }
      }
      if (!targetFunc) {
        size_t funcIdx = getrandom::getRandomIndex(candidateFunctions.size() - 1);
        targetFunc = candidateFunctions[funcIdx];
      }
      
      //Insert using enum declaration inside function body
      if (targetFunc->hasBody()) {
        clang::CompoundStmt* body = cast<clang::CompoundStmt>(targetFunc->getBody());
        SourceLocation insertLoc = body->getBeginLoc().getLocWithOffset(1); // after '{'
        std::string usingDecl = "\nusing enum " + enclosingRecord->getNameAsString() + "::" + targetEnum->getNameAsString() + ";";
        Rewrite.InsertText(insertLoc, usingDecl);
      }
      
      //Collect enum constants
      std::vector<clang::EnumConstantDecl*> constants;
      for (auto* decl : targetEnum->decls()) {
        if (auto* ec = dyn_cast<clang::EnumConstantDecl>(decl)) {
          constants.push_back(ec);
        }
      }
      if (constants.size() < 2) return;
      
      //Choose two constants for compound expression
      size_t c1Idx = getrandom::getRandomIndex(constants.size() - 1);
      size_t c2Idx = getrandom::getRandomIndex(constants.size() - 1);
      if (c1Idx == c2Idx) c2Idx = (c2Idx + 1) % constants.size();
      
      std::string c1Name = constants[c1Idx]->getNameAsString();
      std::string c2Name = constants[c2Idx]->getNameAsString();
      
      //Create compound expression
      std::string compoundExpr = "(" + c1Name + "|" + c2Name + ")";
      
      //Insert new variable declaration using compound expression
      if (targetFunc->hasBody()) {
        clang::CompoundStmt* body = cast<clang::CompoundStmt>(targetFunc->getBody());
        SourceLocation insertLoc = body->getEndLoc().getLocWithOffset(-1); // before '}'
        std::string varDecl = "\nint mut_437_var = " + compoundExpr + ";";
        Rewrite.InsertTextBefore(insertLoc, varDecl);
      }
    }
}
  
void MutatorFrontendAction_437::MutatorASTConsumer_437::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher enumMatcher = enumDecl().bind("EnumDecl");
    DeclarationMatcher funcMatcher = functionDecl().bind("FunctionDecl");
    DeclarationMatcher enumRefMatcher = enumDecl().bind("EnumDeclRef");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(enumMatcher, &callback);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.addMatcher(enumRefMatcher, &callback);
    matchFinder.matchAST(Context);
}