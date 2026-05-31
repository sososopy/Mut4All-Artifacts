```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inject_Invalid_Type_Specifier_291
 */ 
class MutatorFrontendAction_291 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(291)

private:
    class MutatorASTConsumer_291 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_291(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Inject_Invalid_Type_Specifier_291.h"

// ========================================================================================================
#define MUT291_OUTPUT 1

void MutatorFrontendAction_291::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecls")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   VD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      //Replace the original type specifier with an invalid one
      size_t typePos = declaration.find_first_of(" \t");
      if (typePos != string::npos) {
        string originalType = declaration.substr(0, typePos);
        // Randomly select an invalid type specifier
        string invalidType = (getrandom::getRandomIndex(1) == 0) ? "auto" : "void";
        declaration.replace(0, typePos, invalidType);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), declaration);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecls")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      //Replace the original return type with an invalid one
      size_t returnTypePos = declaration.find_first_of(" \t");
      if (returnTypePos != string::npos) {
        string originalReturnType = declaration.substr(0, returnTypePos);
        // Randomly select an invalid return type
        string invalidReturnType = (getrandom::getRandomIndex(1) == 0) ? "auto" : "void";
        declaration.replace(0, returnTypePos, invalidReturnType);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
    }
    else if (auto *TD = Result.Nodes.getNodeAs<clang::TypedefDecl>("TypeDecls")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   TD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      //Replace the original type with an invalid one
      size_t typePos = declaration.find("typedef");
      if (typePos != string::npos) {
        size_t invalidTypePos = declaration.find_first_of(" \t", typePos + 7);
        if (invalidTypePos != string::npos) {
          string originalType = declaration.substr(typePos + 7, invalidTypePos - typePos - 7);
          // Randomly select an invalid type
          string invalidType = (getrandom::getRandomIndex(1) == 0) ? "auto" : "void";
          declaration.replace(typePos + 7, originalType.length(), invalidType);
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_291::MutatorASTConsumer_291::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher varDeclMatcher = varDecl().bind("VarDecls");
    DeclarationMatcher funcDeclMatcher = functionDecl().bind("FuncDecls");
    DeclarationMatcher typeDeclMatcher = typedefDecl().bind("TypeDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varDeclMatcher, &callback);
    matchFinder.addMatcher(funcDeclMatcher, &callback);
    matchFinder.addMatcher(typeDeclMatcher, &callback);
    matchFinder.matchAST(Context);
}