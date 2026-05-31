//header file
#pragma once
#include "Mutator_base.h"

/**
 * ReplaceStringViewWithConstCharPointer_34
 */ 
class MutatorFrontendAction_34 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef File) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_34>(TheRewriter);
    }

private:
    class MutatorASTConsumer_34 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_34(Rewriter &R) : TheRewriter(R) {}
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
};

//source file
#include "../include/ReplaceStringViewWithConstCharPointer_34.h"

// ========================================================================================================
#define MUT34_OUTPUT 1

void MutatorFrontendAction_34::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto params = FD->parameters();
      for (auto param : params) {
        if (auto paramType = param->getType()->getAs<clang::TypedefType>()) {
          if (auto type = paramType->getDecl()->getUnderlyingType()->getAs<clang::BuiltinType>()) {
            if (type->getKind() == clang::BuiltinType::Kind::Char16) {
              //Perform mutation on the source code text by applying string replacement
              std::string replacement = "const char*";
              Rewrite.ReplaceText(param->getSourceRange(), replacement);
            }
          } else if (auto type = paramType->getDecl()->getUnderlyingType()->getAs<clang::RecordType>()) {
            if (type->getDecl()->getNameAsString() == "std::string_view") {
              //Perform mutation on the source code text by applying string replacement
              std::string replacement = "const char*";
              Rewrite.ReplaceText(param->getSourceRange(), replacement);
            }
          }
        }
      }
    } else if (auto *Call = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!Call || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Call->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto args = Call->args();
      for (auto arg : args) {
        if (auto argType = arg->getType()->getAs<clang::TypedefType>()) {
          if (auto type = argType->getDecl()->getUnderlyingType()->getAs<clang::RecordType>()) {
            if (type->getDecl()->getNameAsString() == "std::string_view") {
              //Perform mutation on the source code text by applying string replacement
              std::string replacement = "const char*";
              Rewrite.ReplaceText(arg->getSourceRange(), replacement);
            }
          }
        }
      }
    }
}

void MutatorFrontendAction_34::MutatorASTConsumer_34::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher1 = functionDecl(hasParameter(hasType(asString("std::string_view")))).bind("FunctionDecl");
    CallExprMatcher matcher2 = callExpr(hasArgument(hasType(asString("std::string_view")))).bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}