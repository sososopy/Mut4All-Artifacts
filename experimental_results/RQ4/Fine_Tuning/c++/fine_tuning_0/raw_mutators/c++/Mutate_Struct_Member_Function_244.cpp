//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Struct_Member_Function_244
 */ 
class MutatorFrontendAction_244 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(244)

private:
    class MutatorASTConsumer_244 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_244(Rewriter &R) : TheRewriter(R) {}
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
        
    };
};

//source file
#include "../include/Mutate_Struct_Member_Function_244.h"

// ========================================================================================================
#define MUT244_OUTPUT 1

void MutatorFrontendAction_244::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isStruct())
        return;
      auto methods = MT->methods();
      for (auto method : methods) {
        auto method_name = method->getNameAsString();
        llvm::outs() << method_name << '\n';
        auto method_decl =
            stringutils::rangetoStr(*(Result.SourceManager), method->getSourceRange());
        llvm::outs() << method_decl << '\n';
        auto pos = method_decl.find(method_name);
        method_decl = "static " + method_decl.substr(pos);
        llvm::outs() << method_decl << '\n';
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(method->getSourceRange()), method_decl);
      }
    }
}
  
void MutatorFrontendAction_244::MutatorASTConsumer_244::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}