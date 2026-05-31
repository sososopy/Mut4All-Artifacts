//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Lambda_As_Default_Argument_For_Forward_Declared_Class_Type_384
 */ 
class MutatorFrontendAction_Insert_Lambda_As_Default_Argument_For_Forward_Declared_Class_Type_384 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef File) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_Insert_Lambda_As_Default_Argument_For_Forward_Declared_Class_Type_384>(TheRewriter);
    }

private:
    class MutatorASTConsumer_Insert_Lambda_As_Default_Argument_For_Forward_Declared_Class_Type_384 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Insert_Lambda_As_Default_Argument_For_Forward_Declared_Class_Type_384(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
    Rewriter TheRewriter;
};

//source file
#include "../include/Mutator_Insert_Lambda_As_Default_Argument_For_Forward_Declared_Class_Type_384.h"

// ========================================================================================================
#define MUTINSERT_LAMBDA_AS_DEFAULT_ARGUMENT_FOR_FORWARD_DECLARED_CLASS_TYPE_384_OUTPUT 1

void MutatorFrontendAction_Insert_Lambda_As_Default_Argument_For_Forward_Declared_Class_Type_384::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto params = FD->parameters();
      for (auto param : params) {
        if (auto *PT = dyn_cast<clang::ParmVarDecl>(param)) {
          if (auto *FT = PT->getType()->getAs<clang::PointerType>()) {
            if (auto *CT = FT->getPointeeType()->getAs<clang::RecordType>()) {
              if (CT->isCompleteDefinition()) {
                //Perform mutation on the source code text by applying string replacement
                std::string lambda_expression = "[&](){";
                lambda_expression += "return " + PT->getNameAsString() + ";";
                lambda_expression += "}";
                //Replace the original AST node with the mutated one
                Rewrite.ReplaceText(PT->getSourceRange(), lambda_expression);
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_Insert_Lambda_As_Default_Argument_For_Forward_Declared_Class_Type_384::MutatorASTConsumer_Insert_Lambda_As_Default_Argument_For_Forward_Declared_Class_Type_384::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasParameter(0, parmVarDecl(hasType(pointerType(pointee(recordType()))))).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}