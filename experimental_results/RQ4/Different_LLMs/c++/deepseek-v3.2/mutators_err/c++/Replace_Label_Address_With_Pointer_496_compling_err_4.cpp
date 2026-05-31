//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Label_Address_With_Pointer_496
 */ 
class MutatorFrontendAction_496 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(496)

private:
    class MutatorASTConsumer_496 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_496(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl*> functions;
        bool hasVoidPtrDeclared = false;
        std::string voidPtrVarName;
    };
};

//source file
#include "../include/Mutator_Replace_Label_Address_With_Pointer_496.h"

// ========================================================================================================
#define MUT496_OUTPUT 1

void MutatorFrontendAction_496::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Record function declarations for potential use as replacement pointers
      if (FD->isMain() || FD->hasBody()) {
        functions.push_back(FD);
      }
    }
    else if (auto *UE = Result.Nodes.getNodeAs<clang::UnaryOperator>("LabelAddr")) {
      //Filter nodes in header files
      if (!UE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UE->getBeginLoc()))
        return;
      //Check if this is a GNU address-of-label expression (&&label)
      if (UE->getOpcode() != clang::UO_AddrOf)
        return;
      
      //Get the source code text of target node
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), UE->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      std::string replacement;
      if (!functions.empty()) {
        //Replace with address of a random function from the list
        size_t idx = getrandom::getRandomIndex(functions.size() - 1);
        replacement = "&" + functions[idx]->getNameAsString();
      } else {
        //No function available, use void* pointer
        if (!hasVoidPtrDeclared) {
          //Insert void* variable declaration at the beginning of the function scope
          clang::DeclContext* context = Result.Context->getTranslationUnitDecl();
          if (context && llvm::isa<clang::FunctionDecl>(context)) {
            clang::FunctionDecl* func = llvm::cast<clang::FunctionDecl>(context);
            voidPtrVarName = "mut_ptr_" + std::to_string(getrandom::getRandomIndex(1000));
            std::string decl = "void* " + voidPtrVar2Name + " = ((void*)0);\n";
            Rewrite.InsertText(func->getBody()->getBeginLoc().getLocWithOffset(1), decl);
            hasVoidPtrDeclared = true;
          }
        }
        replacement = voidPtrVarName;
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(UE->getSourceRange()), replacement);
    }
}
  
void MutatorFrontendAction_496::MutatorASTConsumer_496::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher funcMatcher = functionDecl().bind("Functions");
    StatementMatcher labelMatcher = unaryOperator(hasOperatorName("&&")).bind("LabelAddr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.addMatcher(labelMatcher, &callback);
    matchFinder.matchAST(Context);
}