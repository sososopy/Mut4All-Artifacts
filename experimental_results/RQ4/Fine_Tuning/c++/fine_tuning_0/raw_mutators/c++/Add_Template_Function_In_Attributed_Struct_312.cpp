//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Template_Function_In_Attributed_Struct_312
 */ 
class MutatorFrontendAction_312 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(312)

private:
    class MutatorASTConsumer_312 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_312(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Template_Function_In_Attributed_Struct_312.h"

// ========================================================================================================
#define MUT312_OUTPUT 1

void MutatorFrontendAction_312::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ST = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Struct")) {
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;
      if (!ST->isStruct())
        return;
      if (!ST->hasAttrs())
        return;
      if (ST->field_empty())
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), ST->getSourceRange());
      auto fields = ST->fields();
      auto field = fields.begin();
      auto fieldname = (*field)->getNameAsString();
      string func_def = "template <typename T> void mut_312() { (T)" +
                        fieldname + "; }\n";
      content.insert(content.rfind('}'), func_def);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ST->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_312::MutatorASTConsumer_312::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Struct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}