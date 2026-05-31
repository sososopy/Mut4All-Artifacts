//header file
#pragma once
#include "Mutator_base.h"

/**
 * Redefine_Template_Function_With_Different_Return_Type_158
 */ 
class MutatorFrontendAction_158 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(158)

private:
    class MutatorASTConsumer_158 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_158(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<string> return_types;
    };
};

//source file
#include "../include/Redefine_Template_Function_With_Different_Return_Type_158.h"

// ========================================================================================================
#define MUT158_OUTPUT 1

void MutatorFrontendAction_158::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (MT->isTemplated() && MT->isStatic()) {
        auto return_type = MT->getReturnType();
        llvm::outs() << return_type.getAsString() << '\n';
        return_types.push_back(return_type.getAsString());
        auto method_decl =
            stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
        llvm::outs() << method_decl << '\n';
        if (return_type->isVoidType()) {
          method_decl.replace(method_decl.find("void"), 4, "bool");
        } else {
          method_decl.replace(method_decl.find(return_type.getAsString()),
                              return_type.getAsString().length(), "void");
        }
        llvm::outs() << method_decl << '\n';
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()),
                            method_decl);
      }
    }
}
  
void MutatorFrontendAction_158::MutatorASTConsumer_158::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl().bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}