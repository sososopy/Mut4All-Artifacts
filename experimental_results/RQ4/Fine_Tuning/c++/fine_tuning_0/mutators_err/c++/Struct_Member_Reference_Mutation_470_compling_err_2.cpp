//header file
#pragma once
#include "Mutator_base.h"

/**
 * Struct_Member_Reference_Mutation_470
 */ 
class MutatorFrontendAction_470 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(470)

private:
    class MutatorASTConsumer_470 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_470(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Struct_Member_Reference_Mutation_470.h"

// ========================================================================================================
#define MUT470_OUTPUT 1

void MutatorFrontendAction_470::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructWithRefs")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto struct_name = MT->getNameAsString();
      llvm::outs() << struct_name << '\n';
      auto fields = MT->fields();
      for (auto field : fields) {
        if (field->getType()->isReferenceType()) {
          auto field_name = field->getNameAsString();
          auto field_type = field->getType()->getUnqualifiedDesugaredType();
          llvm::outs() << field_name << ' ' << field_type.getAsString() << '\n';
          auto ft = field_type->getAs<RecordType>();
          auto decl = ft->getDecl();
          auto def =
              stringutils::rangetoStr(*(Result.SourceManager), decl->getSourceRange());
          llvm::outs() << def << '\n';
          auto array_type = field_type->getAsArrayTypeUnsafe();
          if (array_type) {
            auto array = array_type->castAs<ConstantArrayType>();
            auto array_size = array->getSize().getZExtValue();
            llvm::outs() << array_size << '\n';
            int new_size = getrandom::getRandomIndex(10) + 1;
            while (new_size == array_size)
              new_size = getrandom::getRandomIndex(10) + 1;
            llvm::outs() << new_size << '\n';
            auto new_def = def.replace(def.find('['), 3, "[" + to_string(new_size) + "][");
            llvm::outs() << new_def << '\n';
            Rewrite.ReplaceText(decl->getSourceRange(), new_def);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_470::MutatorASTConsumer_470::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(has(fieldDecl(hasType(referenceType())))).bind("StructWithRefs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}