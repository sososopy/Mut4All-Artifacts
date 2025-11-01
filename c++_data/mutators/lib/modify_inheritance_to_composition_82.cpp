//source file
#include "../include/modify_inheritance_to_composition_82.h"

// ========================================================================================================
#define MUT82_OUTPUT 1

void MutatorFrontendAction_82::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DerivedDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedStruct")) {
        if (!DerivedDecl || !Result.Context->getSourceManager().isWrittenInMainFile(DerivedDecl->getLocation()))
            return;

        if (DerivedDecl->getNumBases() == 1) {
            auto Base = DerivedDecl->bases_begin();
            if (Base->getType()->isRecordType()) {
                baseClassDecl = Base->getType()->getAsCXXRecordDecl();
                if (!baseClassDecl)
                    return;

                std::string baseName = baseClassDecl->getNameAsString();
                std::string derivedSource = stringutils::rangetoStr(*(Result.SourceManager), DerivedDecl->getSourceRange());

                // Remove the inheritance part
                size_t colonPos = derivedSource.find(':');
                if (colonPos != std::string::npos) {
                    derivedSource.erase(colonPos, derivedSource.find('{') - colonPos);
                }

                // Add composition
                size_t bracePos = derivedSource.find('{');
                if (bracePos != std::string::npos) {
                    derivedSource.insert(bracePos + 1, " " + baseName + " baseMember; ");
                }

                Rewrite.ReplaceText(CharSourceRange::getTokenRange(DerivedDecl->getSourceRange()), derivedSource);
            }
        }
    }
}

void MutatorFrontendAction_82::MutatorASTConsumer_82::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isDerivedFrom(cxxRecordDecl())).bind("DerivedStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}