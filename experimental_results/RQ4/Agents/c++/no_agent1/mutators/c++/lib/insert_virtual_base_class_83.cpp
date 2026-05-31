//source file
#include "../include/insert_virtual_base_class_83.h"

// ========================================================================================================
#define MUT83_OUTPUT 1

void MutatorFrontendAction_83::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClass")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        if ((!RD->isStruct() && !RD->isClass()) || RD->isLambda())
            return;
        if (!RD->isCompleteDefinition())
            return;
        baseClasses.insert(RD);
    } else if (auto *DD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        if (!DD || !Result.Context->getSourceManager().isWrittenInMainFile(DD->getLocation()))
            return;
        if ((!DD->isStruct() && !DD->isClass()) || DD->isLambda())
            return;
        if (!DD->isCompleteDefinition())
            return;

        for (const auto *Base : baseClasses) {
            if (!DD->isDerivedFrom(Base)) {
                std::string baseName = Base->getNameAsString();
                std::string derivedName = DD->getNameAsString();
                std::string insertion = "\n/*mut83*/ class DerivedFrom" + baseName + " : virtual public " + baseName + " {\npublic:\n    DerivedFrom" + baseName + "() {}\n};\n";
                Rewrite.InsertText(DD->getEndLoc(), insertion);
                llvm::outs() << "Inserted virtual base class for " << derivedName << " from " << baseName << "\n";
                break;
            }
        }
    }
}

void MutatorFrontendAction_83::MutatorASTConsumer_83::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher baseMatcher = cxxRecordDecl().bind("BaseClass");
    DeclarationMatcher derivedMatcher = cxxRecordDecl().bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}