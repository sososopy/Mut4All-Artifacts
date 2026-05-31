//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Template_Param_Default_Lambda_137
 */ 
class MutatorFrontendAction_137 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(137)

private:
    class MutatorASTConsumer_137 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_137(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl*> compatibleVars;
        std::vector<const clang::FunctionDecl*> compatibleFuncs;
    };
};

//source file
#include "../include/Mutate_Template_Param_Default_Lambda_137.h"

// ========================================================================================================
#define MUT137_OUTPUT 1

void MutatorFrontendAction_137::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TP = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("TemplateTypeParm")) {
      //Filter nodes in header files
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      //Get the source code text of target node
      auto defaultArg = TP->getDefaultArgument();
      if (!defaultArg.isNull()) return;
      //Check if default argument is a lambda expression
      if (auto *LambdaExpr = dyn_cast<clang::LambdaExpr>(defaultArg.getAsExpr())) {
        //Find the return statement in the lambda body
        auto Body = LambdaExpr->getBody();
        if (!Body) return;
        //Traverse the lambda body to find return statement
        for (auto Stmt : Body->children()) {
          if (auto *ReturnStmt = dyn_cast<clang::ReturnStmt>(Stmt)) {
            auto RetExpr = ReturnStmt->getRetValue();
            if (!RetExpr) return;
            //Get the return type of the lambda
            auto LambdaType = LambdaExpr->getType();
            if (!LambdaType->isFunctionType()) return;
            auto FuncType = LambdaType->getAs<clang::FunctionType>();
            auto RetType = FuncType->getReturnType();
            //Find compatible variables or functions in the seed program
            std::string Replacement;
            //Check compatible variables
            for (auto Var : compatibleVars) {
              if (Var->getType().getCanonicalType() == RetType.getCanonicalType()) {
                Replacement = Var->getNameAsString();
                break;
              }
            }
            //If no compatible variable found, check functions
            if (Replacement.empty()) {
              for (auto Func : compatibleFuncs) {
                if (Func->getReturnType().getCanonicalType() == RetType.getCanonicalType()) {
                  Replacement = Func->getNameAsString() + "()";
                  break;
                }
              }
            }
            //If no compatible element found, create a static variable
            if (Replacement.empty()) {
              Replacement = "static " + RetType.getAsString() + " var{}";
            }
            //Perform mutation on the source code text by applying string replacement
            std::string OriginalText = stringutils::rangetoStr(*(Result.SourceManager), ReturnStmt->getSourceRange());
            std::string MutatedText = "return " + Replacement + ";";
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(ReturnStmt->getSourceRange()), MutatedText);
          }
        }
      }
    }
    //Record compatible variables and functions from the seed program
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("CompatibleVar")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      compatibleVars.push_back(VD);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("CompatibleFunc")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      compatibleFuncs.push_back(FD);
    }
}
  
void MutatorFrontendAction_137::MutatorASTConsumer_137::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateMatcher = templateTypeParmDecl(hasDefaultArgument(lambdaExpr())).bind("TemplateTypeParm");
    DeclarationMatcher varMatcher = varDecl().bind("CompatibleVar");
    DeclarationMatcher funcMatcher = functionDecl().bind("CompatibleFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.matchAST(Context);
}