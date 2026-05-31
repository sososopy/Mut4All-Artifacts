//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Rvalue_Reference_Binding_To_Temporary_In_Constexpr_Member_Function_250
 */ 
class MutatorFrontendAction_250 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(250)

private:
    class MutatorASTConsumer_250 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_250(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl*> enclosingFunctions;
        std::vector<const clang::CXXRecordDecl*> localStructs;
    };
};

//source file
#include "../include/Mutator_250.h"

// ========================================================================================================
#define MUT250_OUTPUT 1

void MutatorFrontendAction_250::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("EnclosingFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      enclosingFunctions.push_back(FD);
    }
    else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("LocalStruct")) {
      //Filter nodes in header files
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      if (!RD->isLocalClass())
        return;
      //Record the node information to be used in the mutation process
      localStructs.push_back(RD);
    }
    else if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Const2exprMethod")) {
      //Filter nodes in header files
      if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MD->getLocation()))
        return;
      if (!MD->isConstexpr())
        return;
      //Get the source code text of target node
      const clang::CXXRecordDecl* structDecl = MD->getParent();
      if (!structDecl || !structDecl->isLocalClass())
         return;
      //Find enclosing function for this local struct
      const clang::FunctionDecl* enclosingFunc = nullptr;
      for (auto* func : enclosingFunctions) {
        clang::SourceLocation funcLoc = func->getBeginLoc();
        clang::SourceLocation structLoc = structDecl->getBeginLoc();
        if (Result.SourceManager->isBeforeInTranslationUnit(funcLoc, structLoc) &&
            Result.SourceManager->isBeforeInTranslationUnit(structLoc, func->getEndLoc())) {
          enclosingFunc = func;
          break;
        }
      }
      if (!enclosingFunc)
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string rvalueRefDecl = "int&& ext = 10;\n";
      //Insert rvalue reference declaration before struct definition
      Rewrite.InsertTextBefore(structDecl->getBeginLoc(), rvalueRefDecl);
      //Change constexpr method body to reference ext
      clang::Stmt* body = MD->getBody2();
      if (body) {
        std::string bodyText = stringutils::rangetoStr(*Result.SourceManager, body->getSourceRange());
        //Find return statement or appropriate place to insert reference
        size_t returnPos = bodyText.find("return");
        if (returnPos != std::string::npos) {
          size_t semicolonPos = bodyText.find(';', returnPos);
          if (semicolonPos != std::string::npos) {
            std::string newBody = bodyText.substr(0, returnPos) + "return ext;" + bodyText.substr(semicolonPos + 1);
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), newBody);
          }
        } else {
          //If no return, add one referencing ext
          std::string newBody = bodyText.substr(0, bodyText.find('}')) + "return ext;\n" + bodyText.substr(bodyText.find('}'));
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), newBody);
        }
      }
    }
}
  
void MutatorFrontendAction_250::MutatorASTConsumer_250::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher enclosingFuncMatcher = functionDecl().bind("EnclosingFunction");
    DeclarationMatcher localStructMatcher = cxxRecordDecl().bind("LocalStruct");
    DeclarationMatcher constexprMethodMatcher = cxxMethodDecl(isConstexpr()).bind("Const2exprMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(enclosingFuncMatcher, &callback);
    matchFinder.addMatcher(localStructMatcher, &callback);
    matchFinder.addMatcher(constexprMethodMatcher, &callback);
    matchFinder.matchAST(Context);
}