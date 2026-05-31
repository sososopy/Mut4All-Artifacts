//header file
#pragma once
#include "Mutator_base.h"

/**
 * Struct_Mutation_Anonymous_Struct_With_Alias_371
 */ 
class MutatorFrontendAction_371 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(371)

private:
    class MutatorASTConsumer_371 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_371(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Struct_Mutation_Anonymous_Struct_With_Alias_371.h"

// ========================================================================================================
#define MUT371_OUTPUT 1

void MutatorFrontendAction_371::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::RecordDecl>("AnonymousStruct")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      llvm::outs() << declaration << '\n';
      if (declaration.find("struct") == string::npos)
        return;
      if (declaration.find("/*mut371*/") != string::npos)
        return;
      if (MT->isAnonymousStructOrUnion() == false)
        return;
      auto fields = MT->fields();
      for (auto field : fields) {
        auto field_type = field->getType();
        auto field_type_str = field_type.getAsString();
        if (field_type_str.find("__") != string::npos) {
          llvm::outs() << field_type_str << '\n';
          auto pos = field_type_str.find("__");
          field_type_str.replace(pos + 2, 4, "unknown_alias");
          llvm::outs() << field_type_str << '\n';
          Rewrite.ReplaceText(field->getTypeSourceInfo()->getTypeLoc(),
                              field_type_str);
        }
      }
    }
}
  
void MutatorFrontendAction_371::MutatorASTConsumer_371::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = recordDecl().bind("AnonymousStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}