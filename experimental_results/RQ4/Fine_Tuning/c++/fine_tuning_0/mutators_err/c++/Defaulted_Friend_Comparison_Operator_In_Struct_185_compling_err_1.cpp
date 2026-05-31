//header file
#pragma once
#include "Mutator_base.h"

/**
 * Defaulted_Friend_Comparison_Operator_In_Struct_185
 */ 
class MutatorFrontendAction_185 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(185)

private:
    class MutatorASTConsumer_185 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_185(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Defaulted_Friend_Comparison_Operator_In_Struct_185.h"

// ========================================================================================================
#define MUT185_OUTPUT 1

void MutatorFrontendAction_185::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (!DL->isStruct())
        return;
      if (DL->isLambda())
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      // llvm::outs()<<content<<'\n';
      auto fields = DL->fields();
      if (fields.empty())
        return;
      auto field = fields.front();
      auto field_type = field->getType();
      if (!field_type->isIntegerType())
        return;
      auto field_name = field->getNameAsString();
      if (field_name.empty())
        return;
      auto struct_name = DL->getNameAsString();
      if (struct_name.empty())
        return;
      auto new_struct_name = struct_name + "_mut185";
      llvm::outs() << new_struct_name << '\n';
      auto new_struct_def = "template<int T>\nstruct " + new_struct_name +
                            " {\nint " + field_name +
                            ";\nfriend bool operator==(const " +
                            new_struct_name +
                            "<T>& lhs, const " + new_struct_name +
                            "<T>& rhs) = default;\n};\n";
      llvm::outs() << new_struct_def << '\n';
      auto new_struct_use = new_struct_name + "<0>";
      llvm::outs() << new_struct_use << '\n';
      llvm::outs() << content << '\n';
      content.insert(content.find(struct_name), new_struct_def);
      stringutils::replace(content, struct_name, new_struct_use);
      llvm::outs() << content << '\n';
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_185::MutatorASTConsumer_185::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}