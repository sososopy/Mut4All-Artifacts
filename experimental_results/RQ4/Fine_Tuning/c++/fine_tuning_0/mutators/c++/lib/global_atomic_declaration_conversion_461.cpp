//source file
#include "../include/Global_Atomic_Declaration_Conversion_461.h"

// ========================================================================================================
#define MUT461_OUTPUT 1

void MutatorFrontendAction_461::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("GlobalAtomic")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      // DL->dump();
      if (!DL->hasGlobalStorage())
        return;
      if (!DL->getType()->isAtomicType())
        return;
      auto type = DL->getType().getAsString();
      llvm::outs() << "type: " << type << '\n';
      auto name = DL->getNameAsString();
      llvm::outs() << "name: " << name << '\n';
      auto atomic_type = DL->getType()->getAs<clang::AtomicType>();
      auto value_type = atomic_type->getValueType().getAsString();
      llvm::outs() << "value_type: " << value_type << '\n';
      auto new_decl = "std::atomic<" + value_type + "> " + name;
      Rewrite.ReplaceText(DL->getSourceRange(), new_decl);
    }
}

void MutatorFrontendAction_461::MutatorASTConsumer_461::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl().bind("GlobalAtomic");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}