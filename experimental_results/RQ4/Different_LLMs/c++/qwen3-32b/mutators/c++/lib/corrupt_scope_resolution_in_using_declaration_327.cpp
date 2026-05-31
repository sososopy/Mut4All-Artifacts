//source file
#include "../include/Corrupt_Scope_Resolution_In_Using_Declaration_327.h"

// ========================================================================================================
#define MUT327_OUTPUT 1

void MutatorFrontendAction_327::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::UsingDecl>("usingDecl")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;
        
        if (!MT->getQualifier() || MT->getName().empty())
            return;
        
        SourceLocation nameLoc = MT->getNameInfo().getBeginLoc();
        if (nameLoc.isInvalid())
            return;
        
        Rewrite.RemoveText(nameLoc, MT->getName().size());
    }
}
  
void MutatorFrontendAction_327::MutatorASTConsumer_327::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = usingDecl(hasDeclContext(cxxRecordDecl())).bind("usingDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}