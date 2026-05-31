//header file
#pragma once
#include "Mutator_base.h"

/**
 * Operator_Struct_Field_Mutation_111
 */ 
class MutatorFrontendAction_111 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(111)

private:
    class MutatorASTConsumer_111 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_111(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::Type *> type_candidates;
    };
};

//source file
#include "../include/operator_struct_field_mutation_111.h"

// ========================================================================================================
#define MUT111_OUTPUT 1

void MutatorFrontendAction_111::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("Fields")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getBeginLoc()))
        return;
      if (FD->isAnonymousStructOrUnion())
        return;
      auto type = FD->getType();
      if (type_candidates.empty())
        return;
      size_t index = getrandom::getRandomIndex(type_candidates.size() - 1);
      auto target_type = type_candidates[index];
      auto type_str =
          stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
      llvm::outs() << type_str << '\n';
      llvm::outs() << target_type->isStructureType() << '\n';
      if (type_str.find("/*mut111*/") != string::npos)
        return;
      if (target_type->isStructureType() || target_type->isUnionType())
        type_str = "/*mut111*/" + target_type->getAsTagDecl()->getNameAsString();
      else
        type_str = "/*mut111*/" + target_type->getCanonicalTypeInternal().getAsString();
      Rewrite.ReplaceText(FD->getBeginLoc(), 0, type_str);
    } else if (auto *TD = Result.Nodes.getNodeAs<clang::TypeDecl>("Types")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getBeginLoc()))
        return;
      if (TD->isCompleteDefinition() == false)
        return;
      auto type = TD->getTypeForDecl();
      if (type->isStructureType() || type->isUnionType())
        type_candidates.push_back(type);
    } else if (auto *BT = Result.Nodes.getNodeAs<clang::BuiltinType>("BuiltIn")) {
      type_candidates.push_back(BT);
    }
}
  
void MutatorFrontendAction_111::MutatorASTConsumer_111::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto field_matcher = fieldDecl().bind("Fields");
    auto type_matcher = typeDecl().bind("Types");
    auto builtin_matcher = builtinType().bind("BuiltIn");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(field_matcher, &callback);
    matchFinder.addMatcher(type_matcher, &callback);
    matchFinder.addMatcher(builtin_matcher, &callback);
    matchFinder.matchAST(Context);
}