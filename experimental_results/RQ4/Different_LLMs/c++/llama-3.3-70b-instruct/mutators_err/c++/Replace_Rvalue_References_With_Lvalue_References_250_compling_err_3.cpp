//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Rvalue_References_With_Lvalue_References_250
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
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Rvalue_References_With_Lvalue_References_250.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/Type.h"
#include "llvm/ADT/APSInt.h"

// ========================================================================================================
#define MUT250_OUTPUT 1

void MutatorFrontendAction_250::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecls")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (MT->getType().getTypePtr()->isRValueReferenceType()) {
        std::string mutatedDeclaration = declaration;
        size_t pos = mutatedDeclaration.find("&&");
        if (pos != std::string::npos) {
          mutatedDeclaration.replace(pos, 2, "&");
          // If the rvalue reference is initialized with an rvalue, introduce a temporary variable
          if (MT->getInit() && isa<clang::IntegerLiteral>(MT->getInit())) {
            auto *Init = cast<clang::IntegerLiteral>(MT->getInit());
            llvm::APSInt value = Init->getValue();
            std::string tempVarName = "temp_" + MT->getNameAsString();
            std::string initValue = value.toString(10, true);
            mutatedDeclaration = "int " + tempVarName + " = " + initValue + ";\n" + mutatedDeclaration;
            mutatedDeclaration.replace(mutatedDeclaration.find("= " + initValue), initValue.length() + 2, tempVarName);
          }
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
        }
      }
    }
}
  
void MutatorFrontendAction_250::MutatorASTConsumer_250::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(referenceType(pointee(isInteger())))).bind("VarDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}